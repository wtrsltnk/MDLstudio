/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/


#include "mdldec.h"


#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )


void CMDLDecompiler::SMD_GenerateReferences()
{

	FILE				*smdfile;
	int					i,j,k,ii,iii,iiii,jj;
	studiohdr_t			*phdr;
	mstudiobodyparts_t	*pbodyparts;
	mstudiomodel_t		*pmodel;
	mstudiomesh_t		*pmesh;
	mstudiotexture_t	*ptexture;
	mstudiobone_t		*pbone;

	short				*ptris;
	short				*pA; // -----> mstudiotrivert_t
	short				*pB; // 
	short				*pC; // 

	char				smdfilename[_MAX_PATH];

	float		bonematrix[3][4];

	vec3_t		BoneEulers;
	vec4_t		q[MAXSTUDIOBONES];


	phdr=(studiohdr_t *)CoreFile;


	pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex);
	for (jj = 0; jj < phdr->numbones ; jj++) 
	{

		BoneEulers[0] = pbone[jj].value[3];
		BoneEulers[1] = pbone[jj].value[4];
		BoneEulers[2] = pbone[jj].value[5];

		AngleQuaternion(BoneEulers, q[jj] );
		QuaternionMatrix( q[jj], bonematrix );

		bonematrix[0][3] = pbone[jj].value[0];
		bonematrix[1][3] = pbone[jj].value[1];
		bonematrix[2][3] = pbone[jj].value[2];

		//FIX: this takes the assumption that the bones are already sorted by parent number in the mdl,
		if (pbone[jj].parent == -1)
		{
			memcpy(g_bonetransform[jj], bonematrix, sizeof(float) * 12);
		} 
		else
		{
			R_ConcatTransforms (g_bonetransform[pbone[jj].parent], bonematrix, g_bonetransform[jj]);
		}
		//g_bonetransform is ready to be used by SMD_WriteTriangle
	}


	for (i = 0; i < phdr->numbodyparts; i++) 
	{
		pbodyparts = (mstudiobodyparts_t *)((byte *)phdr + phdr->bodypartindex + (sizeof(mstudiobodyparts_t) * i));
		for (j = 0; j < pbodyparts->nummodels; j++) 
		{
			pmodel = (mstudiomodel_t *)((byte *)phdr + pbodyparts->modelindex + (sizeof(mstudiomodel_t) * j));
			if (strncmp(pmodel->name, "blank",5) != 0)
			{
				//Write SMD
				char tmpname[64];
				MyExtractFileBase(pmodel->name, tmpname);
				sprintf(smdfilename, "%s%s.smd", DestPath, tmpname);

				smdfile = fopen( smdfilename, "w" );
				if (!smdfile)
				{
					LogMessage (MDLDEC_MSG_ERROR , "ERROR: Can't write %s\r\n", smdfilename);
					//printf( "ERROR: Can't write %s\n", smdfilename);
					return;
				}

				fprintf(smdfile,"version 1\n");

				//Nodes
				SMD_WriteNodes(smdfile);

				//write default position of the nodes
				fprintf(smdfile,"skeleton\n");
				fprintf(smdfile,"time 0\n");
				for (iiii = 0; iiii < phdr->numbones ; iiii++) 
				{
					pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex + (sizeof(mstudiobone_t) * iiii));
					fprintf(smdfile,"%3i %f %f %f",iiii, pbone->value[0], pbone->value[1], pbone->value[2] );
					fprintf(smdfile," %f %f %f\n", pbone->value[3], pbone->value[4], pbone->value[5] );
				}
				fprintf(smdfile,"end\n");

				//Read and decompress OpenGL triangles
				fprintf(smdfile,"triangles\n");

				for (k = 0; k < pmodel->nummesh ; k++) 
				{

					pmesh = (mstudiomesh_t *)((byte *)phdr + pmodel->meshindex + (sizeof(mstudiomesh_t) * k));
					//
					if(phdr->numtextures==0){
						phdr=(studiohdr_t *)TextureFile; // textures are stored in a separated *t.mdl file
					}
					ptexture=(mstudiotexture_t *)((byte *)phdr + phdr->textureindex + sizeof(mstudiotexture_t) * pmesh->skinref);
					phdr=(studiohdr_t *)CoreFile; //restore pointer 					
					//
					ptris = (short *)((byte *)phdr + pmesh->triindex);

					while (ii = *(ptris++))
					{
						
						if (ii < 0)
						{
							//GL_TRIANGLE_FAN :012 023 034 045 056 067 078 089 091
							//inverse GL_TRIANGLE_FAN :021 032 043 054 065 076 087 098 019

							//fprintf(smdfile,"GL_TRIANGLE_FAN : ii %i %s\n",ii,ptexture->name);
							ii = -ii;iii = 0;
							for( ; ii > 0; ii--, ptris += 4, iii++)
							{
								//fprintf(smdfile,"ii %i iii %i vertindex %i normindex %i ",ii,iii,ptris[0],ptris[1]);
								//pA = ptris;
								//SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
								
								switch( iii ) 
								{
									case 0 : pA = ptris; break;
									case 1 : pC = ptris; break;	
									case 2 :
										pB = ptris; //first triangle of fan
										//fprintf(smdfile,"iii %i ",iii);
										SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
										break;
									default:
										pC = pB ; pB = ptris; // pA = no change, pC = pB , pB = new
										//fprintf(smdfile,"iii %i ",iii);
										SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
								}
							}
						}
						else
						{
							//GL_TRIANGLE_STRIP : 012 213 234 435 456 
							// 021 123 243 345 465 567 687
							//fprintf(smdfile,"GL_TRIANGLE_STRIP : ii %i %s\n",ii,ptexture->name);
							iii = 0;
							for( ; ii > 0; ii--, ptris += 4, iii++)
							{
								//fprintf(smdfile,"ii %i iii %i vertindex %i normindex %i ",ii,iii,ptris[0],ptris[1]);
								//pA = ptris;
								//SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
								
								switch( iii ) 
								{
									case 0 : pA = ptris; break;
									case 1 : pC = ptris; break;
									case 2 :
										pB = ptris; //first triangle of strip
										//fprintf(smdfile,"iii %i ",iii);
										SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
										break;
									default:
										if ((iii % 2) == 0)
										{
											pA = pB; pB = ptris; // pA = pC, pB = new , pC = no change 
											//fprintf(smdfile,"iii %i ",iii);
											SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
										}
										else
										{
											pA = pC; pC = ptris; // pA = pC, pB no change, pC = new
											//fprintf(smdfile,"iii %i ",iii);
											SMD_WriteTriangle(smdfile,pmodel,ptexture,pA,pB,pC);
										}
								}
							}
						}
					}
				}
				fprintf(smdfile,"end\n");
				LogMessage (MDLDEC_MSG_INFO , "Reference: %s\r\n", smdfilename);
				//printf( "Reference: %s\n", smdfilename);

				fclose(smdfile);

			}
		}
	}
}


void CMDLDecompiler::SMD_WriteTriangle(FILE *smdfile, mstudiomodel_t *pmodel, mstudiotexture_t *ptexture, short *pA, short *pB, short *pC)
{
	float				s,t;
	studiohdr_t			*phdr;
	byte				*pvertbone;
	byte				*pnormbone;
	vec3_t				*pverts;
	vec3_t				*pnorms;
	vec3_t				refvertex;
	vec3_t				refnormal;

	phdr = (studiohdr_t *)CoreFile;
	pverts = (vec3_t *)((byte *)phdr + pmodel->vertindex);
	pnorms = (vec3_t *)((byte *)phdr + pmodel->normindex);
	pvertbone = ((byte *)phdr + pmodel->vertinfoindex);
	pnormbone = ((byte *)phdr + pmodel->norminfoindex);

	fprintf(smdfile,"%s\n",ptexture->name);

	s = 1.0f/(float)ptexture->width;
	t = 1.0f/(float)ptexture->height;

	//put this in a loop or keep it unrolled?

	VectorTransform (pverts[pA[0]], g_bonetransform[pvertbone[pA[0]]], refvertex);
	VectorRotate ( pnorms[pA[1]], g_bonetransform[pvertbone[pA[0]]], refnormal);
	VectorNormalize ( refnormal );
	//fprintf( smdfile, "%3i %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n"
	fprintf( smdfile, "%3i %f %f %f %f %f %f %f %f\n"
			,pvertbone[pA[0]]
			,refvertex[0], refvertex[1], refvertex[2]
			,refnormal[0], refnormal[1], refnormal[2]
			,pA[2]*s, 1-(pA[3]*t));

	VectorTransform( pverts[pB[0]], g_bonetransform[pvertbone[pB[0]]], refvertex);
	VectorRotate( pnorms[pB[1]], g_bonetransform[pvertbone[pB[0]]], refnormal);
	VectorNormalize( refnormal );
	//fprintf( smdfile, "%3i %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n"
	fprintf( smdfile, "%3i %f %f %f %f %f %f %f %f\n"
			,pvertbone[pB[0]]
			,refvertex[0], refvertex[1], refvertex[2]
			,refnormal[0], refnormal[1], refnormal[2]
			,pB[2]*s, 1-(pB[3]*t));

	VectorTransform( pverts[pC[0]], g_bonetransform[pvertbone[pC[0]]], refvertex);
	VectorRotate( pnorms[pC[1]], g_bonetransform[pvertbone[pC[0]]], refnormal);
	VectorNormalize( refnormal );
	//fprintf( smdfile, "%3i %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n"
	fprintf( smdfile, "%3i %f %f %f %f %f %f %f %f\n"
			,pvertbone[pC[0]]
			,refvertex[0], refvertex[1], refvertex[2]
			,refnormal[0], refnormal[1], refnormal[2]
			,pC[2]*s, 1-(pC[3]*t));
}

void CMDLDecompiler::SMD_GenerateSequences()
{
	FILE				*smdfile;
	char				smdfilename[_MAX_PATH];
	int					i,j,ii;
	studiohdr_t			*phdr;
	mstudioseqdesc_t	*pseqdesc;
	mstudioanim_t		*panim;
	mstudioseqgroup_t	*pseqgroup;

	phdr = (studiohdr_t *)CoreFile;

	//for each sequence extract the frames
	for (i = 0; i < phdr->numseq; i++) 
	{
		pseqdesc = (mstudioseqdesc_t *)((byte *)phdr + phdr->seqindex + (sizeof(mstudioseqdesc_t) * i));
		pseqgroup = (mstudioseqgroup_t *)((byte *)phdr + phdr->seqgroupindex) + pseqdesc->seqgroup;

		for (ii = 0; ii < pseqdesc->numblends; ii++) 
		{
			//get anim from sequence files if needed (What is pseqgroup->data?)
			if (pseqdesc->seqgroup == 0)
			{
				panim =  (mstudioanim_t *)((byte *)phdr + pseqgroup->data + pseqdesc->animindex);
			}
			else
			{
				panim = (mstudioanim_t *)((byte *)m_panimhdr[pseqdesc->seqgroup] + pseqdesc->animindex);
			}
			panim += (phdr->numbones * ii); //points panim by current blend
			
			//set filename

			if (pseqdesc->numblends == 1)
			{
				sprintf(smdfilename, "%s%s.smd", DestPath, pseqdesc->label);
			}
			else
			{
				sprintf(smdfilename, "%s%s_blend%i.smd", DestPath, pseqdesc->label, ii+1);
			}
			// open 
			
			smdfile = fopen( smdfilename, "w" );
			if (!smdfile)
			{
				LogMessage (MDLDEC_MSG_ERROR , "ERROR: Can't write %s\r\n", smdfilename);
				//printf( "ERROR: Can't write %s\n", smdfilename);
				return;
			}

			fprintf(smdfile,"version 1\n");
			SMD_WriteNodes(smdfile);
			fprintf(smdfile,"skeleton\n");

			//printf("%s \n", smdfilename);
			//printf("sequence %s seqgroup %i animindex %i frames %i fps %.0f\n", pseqdesc->label, pseqdesc->seqgroup, pseqdesc->animindex, pseqdesc->numframes, pseqdesc->fps);
			//printf("cache %i data %i name %s label %s\n", pseqgroup->cache , pseqgroup->data, pseqgroup->label,pseqgroup->name);
			
			for (j = 0; j < pseqdesc->numframes; j++) 
			{
				SMD_WriteFrame( smdfile, j, panim, pseqdesc);
			}

			fprintf(smdfile,"end\n");
			fclose(smdfile);
			LogMessage (MDLDEC_MSG_INFO , "Sequence: %s\r\n", smdfilename);
			//printf( "Sequence: %s\n", smdfilename);
		}
	}
}

void CMDLDecompiler::SMD_WriteFrame(FILE *smdfile, int frame, mstudioanim_t *panim, mstudioseqdesc_t *pseqdesc)
{
	int					j,k,kf;
	studiohdr_t			*phdr;
	mstudiobone_t		*pbone;
	mstudioanimvalue_t	*panimvalue;

	vec3_t				pos;
	vec3_t				Eulers;
	vec3_t				angle1, angle2;
	float				s;

	s = 0.0;

	phdr = (studiohdr_t *)CoreFile;
	pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex);

	fprintf(smdfile,"time %i\n",frame);

	for (k = 0; k < phdr->numbones; k++, pbone++, panim++) 
	{
		// pos loop 
		for (j = 0; j < 3; j++)
		{
			pos[j] = pbone->value[j]; // default;
			if (panim->offset[j] != 0)
			{
				panimvalue = (mstudioanimvalue_t *)((byte *)panim + panim->offset[j]);
				
				kf = frame;
				// find span of values that includes the frame we want
				while (panimvalue->num.total <= kf)
				{
					kf -= panimvalue->num.total;
					panimvalue += panimvalue->num.valid + 1;
				}
				// if we're inside the span
				if (panimvalue->num.valid > kf)
				{
					// and there's more data in the span
					if (panimvalue->num.valid > kf + 1)
					{
						pos[j] += (panimvalue[kf+1].value * (1.0 - s) + s * panimvalue[kf+2].value)  * pbone->scale[j];
					}
					else
					{
						pos[j] += panimvalue[kf+1].value * pbone->scale[j];
					}
				}
				else
				{
					// are we at the end of the repeating values section and there's another section with data?
					if (panimvalue->num.total <= kf + 1)
					{
						pos[j] += (panimvalue[panimvalue->num.valid].value * (1.0 - s) + s * panimvalue[panimvalue->num.valid + 2].value) * pbone->scale[j];
					}
					else
					{
						pos[j] += panimvalue[panimvalue->num.valid].value  * pbone->scale[j];
					}
				}
			}
		}

		//Eulers loop
		for (j = 0; j < 3; j++)
		{
			if (panim->offset[j+3] == 0)
			{
				angle2[j] = angle1[j] = pbone->value[j+3]; // default;
			}
			else
			{
				panimvalue = (mstudioanimvalue_t *)((byte *)panim + panim->offset[j+3]);
				kf = frame;
				while (panimvalue->num.total <= kf)
				{
					kf -= panimvalue->num.total;
					panimvalue += panimvalue->num.valid + 1;
				}
				// Bah, missing blend!
				if (panimvalue->num.valid > kf)
				{
					angle1[j] = panimvalue[kf+1].value;

					if (panimvalue->num.valid > kf + 1)
					{
						angle2[j] = panimvalue[kf+2].value;
					}
					else
					{
						if (panimvalue->num.total > kf + 1)
							angle2[j] = angle1[j];
						else
							angle2[j] = panimvalue[panimvalue->num.valid+2].value;
					}
				}
				else
				{
					angle1[j] = panimvalue[panimvalue->num.valid].value;
					if (panimvalue->num.total > kf + 1)
					{
						angle2[j] = angle1[j];
					}
					else
					{
						angle2[j] = panimvalue[panimvalue->num.valid + 2].value;
					}
				}
				angle1[j] = pbone->value[j+3] + angle1[j] * pbone->scale[j+3];
				angle2[j] = pbone->value[j+3] + angle2[j] * pbone->scale[j+3];
			}
		}

		/*

		vec4_t				q, q1, q2;

		if (!VectorCompare( angle1, angle2 ))
		{
			AngleQuaternion( angle1, q1 );
			AngleQuaternion( angle2, q2 );
			QuaternionSlerp( q1, q2, s, q );
		}
		else
		{
			AngleQuaternion( angle1, q );
		}

		Eulers[0] = angle1[0];
		Eulers[1] = angle1[1];
		Eulers[2] = angle1[2];

		*/

		VectorCopy( angle1, Eulers);

		if (pbone->parent == -1)
		{
			vec3_t		temppos;
			
			//restore linear movement for sequences with
			//STUDIO_LX, STUDIO_LY and STUDIO_LZ
			VectorScale( pseqdesc->linearmovement, frame * 1.0 / pseqdesc->numframes, temppos );
			VectorAdd( pos, temppos, pos );

			//studiomdl rotates sequence smds +90 in the z axis,
			//see Grab_Animation in studiomdl.c
			//FIX: find a more elegant way to do this.

			float		zrotation, cz, sz;

			zrotation = -90.0 * (Q_PI / 180.0);
			cz = cos( zrotation );
			sz = sin( zrotation );
			VectorCopy(pos, temppos);

			pos[0] = cz * temppos[0] - sz * temppos[1];
			pos[1] = sz * temppos[0] + cz * temppos[1];
			pos[2] = temppos[2];
			Eulers[2] += zrotation; 

			//clip_rotations(Eulers);

			// This is [void clip_rotations( vec3_t rot )] taken from studiomdl.c :
			// clip everything to : -Q_PI <= x < Q_PI
			int jj;
			for (jj = 0; jj < 3; jj++) {
				while (Eulers[jj] >= Q_PI) 
					Eulers[jj] -= Q_PI*2;
				while (Eulers[jj] < -Q_PI) 
					Eulers[jj] += Q_PI*2;
			}

		} 
		
		fprintf(smdfile,"%3i   " ,k);
		fprintf(smdfile,"%f %f %f %f %f %f\n" ,pos[0],pos[1],pos[2],Eulers[0],Eulers[1],Eulers[2]);
	}
}

void CMDLDecompiler::SMD_WriteNodes(FILE *smd)
{
	int					i;
	studiohdr_t			*phdr;
	mstudiobone_t		*pbone;

	phdr=(studiohdr_t *)CoreFile;
	//write nodes
	fprintf(smd,"nodes\n");
	for (i = 0; i < phdr->numbones ; i++) 
	{
		pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex + (sizeof(mstudiobone_t) * i));
		fprintf(smd,"%3i \"%s\" %i\n", i , pbone->name , pbone->parent );
	}
	fprintf(smd,"end\n");
}

