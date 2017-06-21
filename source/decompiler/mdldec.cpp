/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/


#include "mdldec.h"


CMDLDecompiler::CMDLDecompiler()
{

	int i;
	strset(ModelName,0);
	strset(DestPath,0);

	ModelLoaded = false;
	CoreFile = NULL;
	CoreFileLength = 0;

	for(i=0;i<32;i++)
	{
		SequenceFilesLength[i] = 0;
		SequenceFiles[i] = NULL;
	}

	TextureFile =  NULL;
	TextureFileLength=0;

}


CMDLDecompiler::~CMDLDecompiler()
{

	int i;

	if(CoreFileLength>0)
	{
		free(CoreFile);
	}
	for(i=0;i<32;i++)
	{
		if(SequenceFilesLength[i]>0)
		{
			free(SequenceFiles[i]);
		}
	}
	if(TextureFileLength>0)
	{
		free(TextureFile);
	}
	
}

bool CMDLDecompiler::LoadModel( char *modelname )
{ 
	// This function loads the modeldata. This can be up to a lot of files (32 sequence
	// files, 1 core file and 1 additional texturefile) The memory allocated for these
	// files are stored separately in memoryancher pointers. All pointers in the
	// MDL point inside that memory.

	FILE				*fp;
	byte				*pin;
	studiohdr_t			*phdr;
	mstudiotexture_t	*ptexture;
	char				texturename[256];
	char				seqgroupname[256];



	//destpath
	if ( DestPath[0] )
	{
		char	*ofs, c;
		if ((DestPath [strlen(DestPath)-1] != '/') || (DestPath [strlen(DestPath)-1] != '\\')) 
			strcat(DestPath, "\\");

		for (ofs = DestPath ; *ofs ; ofs++)
		{
			c = *ofs;
			if (c == '/' || c == '\\')
			{	// create the directory
				*ofs = 0;
				if (_mkdir (DestPath) == -1)
				{
					if (errno != 17 ) //EEXIST 
					{
						LogMessage (MDLDEC_MSG_ERROR , "ERROR: Couln't create %s\r\n", DestPath);
						return false;
					}
				}
				*ofs = c;
			}
		}	
	}
	else
	{
		MyExtractFilePath( modelname, DestPath);
	}

	// load core file
	if( (fp = fopen( modelname, "rb" )) == NULL)
	{
		LogMessage (MDLDEC_MSG_ERROR , "ERROR: Couln't open %s\r\n",modelname);
		//printf ("\nERROR: Couln't open %s\n",modelname);
		return false;
	}
	fseek( fp, 0, SEEK_END );
	CoreFileLength = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	CoreFile = (byte *)malloc(CoreFileLength);
	fread(CoreFile, CoreFileLength, 1, fp ); 
	fclose(fp);

	// test for the IDST tag...
	if(memcmp(CoreFile,"IDST",4)!=0)
	{
		// bad modelfile.
		free(CoreFile);
		CoreFileLength=0;
		LogMessage (MDLDEC_MSG_ERROR , "ERROR: %s isn't is a valid HL model file.\r\n", modelname);
		//printf ("\nERROR: %s is not a HL model file.\n",modelname);
		return false;
	}

	m_pstudiohdr = (studiohdr_t *)CoreFile;

	// init the pointers to point to the data inside the corefile in memory.
	pin = CoreFile;
	phdr = (studiohdr_t *)pin;

	ptexture = (mstudiotexture_t *)(pin + phdr->textureindex);

	// load textures file if any
	if (m_pstudiohdr->numtextures == 0)
	{
		strcpy( texturename, modelname );
		strcpy( &texturename[strlen(texturename) - 4], "T.mdl" );

		if( (fp = fopen( texturename, "rb" )) == NULL)
		{
			LogMessage (MDLDEC_MSG_ERROR , "ERROR: Missing textures file %s\r\n",texturename);
			//printf ("\nERROR: Missing textures file %s\n",texturename);
			return false;
		}
		fseek( fp, 0, SEEK_END );
		TextureFileLength = ftell( fp );
		fseek( fp, 0, SEEK_SET );
		TextureFile = (byte *)malloc(TextureFileLength);
		fread(TextureFile, TextureFileLength, 1, fp );
		fclose(fp);

		m_ptexturehdr = (studiohdr_t *)TextureFile;
		
	}
	else
	{
		m_ptexturehdr = m_pstudiohdr;
	}

	// load sequence files if any
	if (m_pstudiohdr->numseqgroups > 1)
	{
		for (int i = 1; i < m_pstudiohdr->numseqgroups; i++)
		{
			strcpy( seqgroupname, modelname );
			sprintf( &seqgroupname[strlen(seqgroupname) - 4], "%02d.mdl", i );	

			if( (fp = fopen( seqgroupname, "rb" )) == NULL)
			{
				LogMessage (MDLDEC_MSG_ERROR , "ERROR: Couln't open sequence file %s\n",seqgroupname);
				//printf ("\nERROR: Couln't open sequence file %s\n",seqgroupname);
				return false;
			}

			fseek( fp, 0, SEEK_END );
			SequenceFilesLength[i] = ftell( fp );
			fseek( fp, 0, SEEK_SET );
			SequenceFiles[i] = (byte *)malloc(SequenceFilesLength[i]);
			fread(SequenceFiles[i], SequenceFilesLength[i], 1, fp );
			fclose(fp);

			m_panimhdr[i] = (studioseqhdr_t *)SequenceFiles[i];
		}
	}

	return true;
}

bool CMDLDecompiler::LoadData( byte *pCoreFile, int nCoreFileLength, 
							  byte *pTextureFile, int nTextureFileLength, 
							  byte *pSequenceFiles[], int nSequenceFilesLength[] )
{
	CoreFile = pCoreFile;
	CoreFileLength = nCoreFileLength;

	TextureFile = pTextureFile;
	TextureFileLength = nTextureFileLength;

	for ( int i=0; i < 32; i++ )
	{
		SequenceFiles[i] = pSequenceFiles[i];
		SequenceFilesLength[i] = nSequenceFilesLength[i];
	}
	return true;
}

void CMDLDecompiler::SetDestPath( char *modelname )
{
	char path[MAX_PATH];
	char base[MAX_PATH];

	MyExtractFileBase( modelname, base );
	MyExtractFilePath( modelname, path );

	strcpy(DestPath, path);
	strcat(DestPath, base);
}

void CMDLDecompiler::DumpInfo()
{

	FILE *modelinfo;
	byte				*pin;
	studiohdr_t			*phdr;

	pin = CoreFile;
	phdr = (studiohdr_t *)pin;
	modelinfo = fopen( "unmdl.txt", "w" );


	fprintf (modelinfo,"name: %s\n\n",m_pstudiohdr->name);

	fprintf (modelinfo,"flags: %i\n",m_pstudiohdr->flags);

	fprintf (modelinfo,"numbones: %i\n",m_pstudiohdr->numbones);
	fprintf (modelinfo,"boneindex: %i\n",phdr->boneindex);

	fprintf (modelinfo,"numbonecontrollers: %i\n",phdr->numbonecontrollers);
	fprintf (modelinfo,"bonecontrollerindex: %i\n",phdr->bonecontrollerindex);

	fprintf (modelinfo,"numhitboxes: %i\n",phdr->numhitboxes);
	fprintf (modelinfo,"hitboxindex: %i\n",phdr->hitboxindex);

	fprintf (modelinfo,"numseq: %i\n",phdr->numseq);
	fprintf (modelinfo,"seqindex: %i\n",phdr->seqindex);

	fprintf (modelinfo,"numseqgroups: %i\n",phdr->numseqgroups);
	fprintf (modelinfo,"seqgroupindex: %i\n",phdr->seqgroupindex);

	if(m_pstudiohdr->numtextures == 0)
	{
		fprintf (modelinfo,"numtextures: %i\n",m_ptexturehdr->numtextures);
		fprintf (modelinfo,"textureindex: %i\n",m_ptexturehdr->textureindex);
		fprintf (modelinfo,"texturedataindex: %i\n",m_ptexturehdr->texturedataindex);
		fprintf (modelinfo,"numskinref: %i\n",m_ptexturehdr->numskinref);
		fprintf (modelinfo,"numskinfamilies: %i\n",m_ptexturehdr->numskinfamilies);
		fprintf (modelinfo,"skinindex: %i\n",m_ptexturehdr->skinindex);
	}
	else
	{
		fprintf (modelinfo,"numtextures: %i\n",phdr->numtextures);
		fprintf (modelinfo,"textureindex: %i\n",phdr->textureindex);
		fprintf (modelinfo,"texturedataindex: %i\n",phdr->texturedataindex);
		fprintf (modelinfo,"numskinref: %i\n",phdr->numskinref);
		fprintf (modelinfo,"numskinfamilies: %i\n",phdr->numskinfamilies);
		fprintf (modelinfo,"skinindex: %i\n",phdr->skinindex);
	}

	fprintf (modelinfo,"numbodyparts: %i\n",phdr->numbodyparts);
	fprintf (modelinfo,"bodypartindex: %i\n",phdr->bodypartindex);

	fprintf (modelinfo,"numattachments: %i\n",phdr->numattachments);
	fprintf (modelinfo,"attachmentindex: %i\n",phdr->attachmentindex);

	fprintf (modelinfo,"transitions: %i\n",phdr->numtransitions);
	fprintf (modelinfo,"transitionindex: %i\n",phdr->transitionindex);

	fclose(modelinfo);

}

void CMDLDecompiler::GetModelName()
{
	char *src;
	char *pname;
	int	 c;

	c=0;
	pname = (char *)m_pstudiohdr->name;
	src = pname + strlen(pname) - 1;
	while (src != pname && !(*(src-1) == '\\' || *(src-1) == '/'))
	{
		src--;	// back up until a \ or the start
		c++;
	};

	strncpy(ModelName,src,c-3);

}

void CMDLDecompiler::FixRepeatedSequenceNames()
{
	int					seqcount;
	int					i,ii;
	studiohdr_t			*phdr;
	mstudioseqdesc_t	*pseqdesc;
	mstudioseqdesc_t	*pseqdesc2;

	phdr = (studiohdr_t *)CoreFile;

	//loop sequences
	for (i = 0; i < phdr->numseq; i++) 
	{
		pseqdesc = (mstudioseqdesc_t *)((byte *)phdr + phdr->seqindex + (sizeof(mstudioseqdesc_t) * i));
		seqcount = 1;

		for (ii = 0; ii < phdr->numseq; ii++) 
		{
			if (ii != i) 
			{
				pseqdesc2 = (mstudioseqdesc_t *)((byte *)phdr + phdr->seqindex + (sizeof(mstudioseqdesc_t) * ii));
				if (strncmp(pseqdesc2->label, pseqdesc->label,sizeof(pseqdesc->label)) == 0)
				{
					//Add a sufix to sequence name
					seqcount++ ;
					sprintf(pseqdesc2->label, "%s_%i", pseqdesc2->label, seqcount);
				}			
			}
		}

		if (seqcount > 1)
		{
			LogMessage (MDLDEC_MSG_WARNING , "WARNING: Sequence name \"%s\" is repeated %i times.\r\n", pseqdesc->label, seqcount);
			//printf("WARNING: Sequence name \"%s\" is repeated %i times.\n", pseqdesc->label, seqcount);
			//printf("         Added numeric suffix to avoid overwrite.\n");
			sprintf(pseqdesc->label, "%s_1",pseqdesc->label);
		}
	}
}
