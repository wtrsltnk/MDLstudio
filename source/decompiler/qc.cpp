/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/


#include "mdldec.h"


void CMDLDecompiler::QC_GenerateScript()
{
	char	qcfilename[_MAX_PATH];
	char	path[_MAX_PATH];

	GetModelName();
	sprintf(qcfilename, "%s%s.qc", DestPath, ModelName);

	//if (_getcwd( path, _MAX_PATH) == NULL)
	//{
	//	if ((errno == 12) || (errno == 34))  //printf("ENOMEM");printf("ERANGE");
	//	printf("WARNING: Can't get current directory path, using \".\\\" in .qc file.");
		strcpy(path, ".");
	//}

	//create qc file
	qcfile = fopen( qcfilename, "w" );
	if (!qcfile)
	{
		LogMessage (MDLDEC_MSG_ERROR , "ERROR: Can't write %s\r\n", qcfilename);
		//printf( "ERROR: Can't write %s\n", qcfilename);
		return;
	}

	fprintf(qcfile,"/*\n");
	fprintf(qcfile,"==============================================================================\n\n");
	fprintf(qcfile,"QC script generated by Half-Life MDL Decompiler v1.01.\n");
	fprintf(qcfile,"2002, Kratisto. Based on code from Valve's HL SDK.\n\n");
	fprintf(qcfile,"%s.qc\n",ModelName);
	fprintf(qcfile,"Original model name: %s\n\n",m_pstudiohdr->name);
	fprintf(qcfile,"==============================================================================\n");
	fprintf(qcfile,"*/\n\n");

	//main
	//fprintf(qcfile,"$modelname \"%s\\%s.mdl\"\n",path,ModelName);
	fprintf(qcfile,"$modelname \"%s.mdl\"\n",ModelName);
	fprintf(qcfile,"$cd \"%s\\\"\n",path);
	fprintf(qcfile,"$cdtexture \"%s\\\"\n",path);
	fprintf(qcfile,"$scale 1.0\n");
	fprintf(qcfile,"\n");

	if(m_pstudiohdr->numtextures == 0)
	{
		fprintf(qcfile,"$externaltextures\n");
	}

	if(m_pstudiohdr->flags != 0 )
	{
		fprintf(qcfile,"$flags %i\n",m_pstudiohdr->flags);
		LogMessage (MDLDEC_MSG_WARNING ,"WARNING: This model uses the $flags keyword set to %i\r\n",m_pstudiohdr->flags);
		//printf("WARNING: This model uses the $flags keyword set to %i\n",m_pstudiohdr->flags);
	}
	fprintf(qcfile,"\n");
	
	fprintf(qcfile,"$bbox %f %f %f",m_pstudiohdr->min[0],m_pstudiohdr->min[1],m_pstudiohdr->min[2]);
	fprintf(qcfile," %f %f %f\n",m_pstudiohdr->max[0],m_pstudiohdr->max[1],m_pstudiohdr->max[2]);

	fprintf(qcfile,"$cbox %f %f %f",m_pstudiohdr->bbmin[0],m_pstudiohdr->bbmin[1],m_pstudiohdr->bbmin[2]);
	fprintf(qcfile," %f %f %f\n",m_pstudiohdr->bbmax[0],m_pstudiohdr->bbmax[1],m_pstudiohdr->bbmax[2]);
	
	fprintf(qcfile,"\n");
	fprintf(qcfile,"$eyeposition %f %f %f\n",m_pstudiohdr->eyeposition[0],m_pstudiohdr->eyeposition[1],m_pstudiohdr->eyeposition[2]);
	
	//body meshes
	QC_WriteBodyModels();
	//texturegroup
	QC_WriteTextureGroups();
	//attachments
	QC_WriteAttachments();
	//controllers
	QC_WriteBoneControllers();
	//hitboxes
	QC_WriteHitBoxes();
	//sequences
	QC_WriteSequences();
	//close qc file
	fprintf(qcfile,"\n// End of QC file.\n");
	LogMessage (MDLDEC_MSG_INFO , "QC Script: %s\r\n", qcfilename);
	//printf( "QC script: %s\n", qcfilename);
	fclose(qcfile);

}


void CMDLDecompiler::QC_WriteTextureGroups ()
{
	int					i;
	int					j;
	int					ii;
	short				*pskinref;
	short				*pskinindex;
	studiohdr_t			*phdr;
	mstudiotexture_t	*pTexture;

	phdr=(studiohdr_t *)CoreFile;
	if(phdr->numtextures==0)
	{
		phdr=(studiohdr_t *)TextureFile;
	}

	if(phdr->numskinfamilies > 1)
	{

		fprintf(qcfile,"\n// %i skin families\n",phdr->numskinfamilies);

		fprintf (qcfile,"$texturegroup skinfamilies \n{\n");
		pskinindex = (short *)((byte *)phdr + phdr->skinindex);
		for (i = 0; i < phdr->numskinfamilies; i++) 
		{
			fprintf (qcfile,"{");
			for (j = 0; j < phdr->numskinref; j++) 
			{
				pskinref = pskinindex + (i * phdr->numskinref) + j ;
				for (ii = 0; ii < phdr->numskinfamilies; ii++) 
				{
					if (*pskinref != *(pskinindex + (ii * phdr->numskinref) + j ))
					{
						pTexture=(mstudiotexture_t *)((byte *)phdr + phdr->textureindex + sizeof(mstudiotexture_t) * *pskinref);
						fprintf (qcfile," \"%s\" ",pTexture->name);
						break;
					}
				}
			}
			fprintf (qcfile,"}\n");
		}	
		fprintf (qcfile,"}\n");
	}
}

void CMDLDecompiler::QC_WriteBodyModels()
{
	int					i;
	int					j;
	studiohdr_t			*phdr;
	mstudiobodyparts_t	*pbodyparts;
	mstudiomodel_t		*pmodel;

	fprintf(qcfile,"\n// reference meshes\n");

	phdr=(studiohdr_t *)CoreFile;

	for (i = 0; i < phdr->numbodyparts; i++) 
	{
		pbodyparts = (mstudiobodyparts_t *)((byte *)phdr + phdr->bodypartindex + (sizeof(mstudiobodyparts_t) * i));
		if (pbodyparts->nummodels == 1)
		{
			pmodel = (mstudiomodel_t *)((byte *)phdr + pbodyparts->modelindex);

			MyExtractFileBase(pmodel->name,pmodel->name);

			fprintf(qcfile,"$body %s \"%s\"\n\n",pbodyparts->name,pmodel->name);
		}
		else
		{
			fprintf(qcfile,"$bodygroup %s\n",pbodyparts->name);
			fprintf(qcfile,"{\n");
			for (j = 0; j < pbodyparts->nummodels; j++) 
			{
				pmodel = (mstudiomodel_t *)((byte *)phdr + pbodyparts->modelindex + (sizeof(mstudiomodel_t) * j));
				if (strncmp(pmodel->name, "blank",5) == 0)
				{
					fprintf(qcfile,"blank\n");
				}
				else
				{
					MyExtractFileBase(pmodel->name,pmodel->name);
					fprintf(qcfile,"studio \"%s\"\n",pmodel->name);
				}
			}
			fprintf(qcfile,"}\n");
		}
	}
}

void CMDLDecompiler::QC_WriteAttachments()
{
	int					i;
	studiohdr_t			*phdr;
	mstudioattachment_t *pattachment;
	mstudiobone_t		*pbone;

	phdr=(studiohdr_t *)CoreFile;
	fprintf(qcfile,"\n// %i attachments\n",phdr->numattachments);

	for (i = 0; i < phdr->numattachments  ; i++) 
	{
		pattachment = (mstudioattachment_t *)((byte *)phdr + phdr->attachmentindex + (sizeof(mstudioattachment_t) * i));
		pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex + (sizeof(mstudiobone_t) * pattachment->bone));
		fprintf(qcfile,"$attachment %i \"%s\" %f %f %f\n",i,pbone->name,pattachment->org[0],pattachment->org[1],pattachment->org[2]);
	}
}

void CMDLDecompiler::QC_WriteBoneControllers()
{
	int						i;
	studiohdr_t				*phdr;
	mstudiobonecontroller_t *pbctrl;
	mstudiobone_t			*pbone;
	char					mflagstring[5];

	phdr=(studiohdr_t *)CoreFile;
	fprintf(qcfile,"\n// %i bone controllers\n",phdr->numbonecontrollers);

	for (i = 0; i < phdr->numbonecontrollers  ; i++) 
	{
		pbctrl = (mstudiobonecontroller_t *)((byte *)phdr + phdr->bonecontrollerindex + (sizeof(mstudiobonecontroller_t) * i));
		pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex + (sizeof(mstudiobone_t) * pbctrl->bone));

		fprintf(qcfile,"$controller ");

		if (pbctrl->index == 4)
		{
			fprintf(qcfile,"mouth ");
		}
		else
		{
			fprintf(qcfile,"%i ",pbctrl->index);
		}
		//Sometimes, pbone->name  returns "(null)" (???) if is called after QC_TranslateMotionFlag,
		//I don't know why. This happened with sentry.mdl. 
		fprintf(qcfile,"\"%s\" ",pbone->name);
		//
		QC_TranslateMotionFlag((pbctrl->type & ~STUDIO_RLOOP), mflagstring, false);
		//fprintf(qcfile,"%s\n",pbone->name);

		fprintf( qcfile, "%s %f %f\n", mflagstring, pbctrl->start, pbctrl->end );
	}
}

void CMDLDecompiler::QC_WriteHitBoxes()
{
	int						i;
	studiohdr_t				*phdr;
	mstudiobbox_t			*phbox;
	mstudiobone_t			*pbone;

	phdr=(studiohdr_t *)CoreFile;
	fprintf(qcfile,"\n// %i hit boxes\n",phdr->numhitboxes);

	for (i = 0; i < phdr->numhitboxes; i++) 
	{
		phbox = (mstudiobbox_t *)((byte *)phdr + phdr->hitboxindex + (sizeof(mstudiobbox_t) * i));
		pbone = (mstudiobone_t *)((byte *)phdr + phdr->boneindex + (sizeof(mstudiobone_t) * phbox->bone));
		fprintf(qcfile,"$hbox %i \"%s\" %f %f %f %f %f %f\n", phbox->group, pbone->name, phbox->bbmin[0], phbox->bbmin[1], phbox->bbmin[2], phbox->bbmax[0], phbox->bbmax[1], phbox->bbmax[2]);
	}
}

void CMDLDecompiler::QC_WriteSequences()
{
	int						i;
	int						j;
	char					flagstring[32];
	studiohdr_t				*phdr;
	mstudioseqdesc_t		*pseqdesc;
	mstudioevent_t			*pevent;

	phdr=(studiohdr_t *)CoreFile;
	fprintf(qcfile,"\n// %i animation sequences\n",phdr->numseq);

	for (i = 0; i < phdr->numseq; i++) 
	{
		pseqdesc = (mstudioseqdesc_t *)((byte *)phdr + phdr->seqindex + (sizeof(mstudioseqdesc_t) * i));
		fprintf(qcfile,"$sequence \"%s\" ", pseqdesc->label);
		if (pseqdesc->numblends > 1)
		{
			//uses blends, put smds
			if (pseqdesc->numblends > 2)
				fprintf(qcfile,"{\n");

			for (j = 0; j < pseqdesc->numblends; j++) 
			{
				if (pseqdesc->numblends > 2)
					fprintf(qcfile,"          ");

				fprintf(qcfile,"\"%s_blend%i\" ", pseqdesc->label, j+1);

				if (pseqdesc->numblends > 2)
					fprintf(qcfile,"\n");
			}

			if (pseqdesc->numblends > 2)
				fprintf(qcfile,"          ");

			QC_TranslateMotionFlag(pseqdesc->blendtype[0], flagstring, false);
			fprintf(qcfile,"blend %s %.0f %.0f", flagstring,pseqdesc->blendstart[0],pseqdesc->blendend[0]);
		}
		else
		{
			//normal
			fprintf(qcfile,"\"%s\"", pseqdesc->label);
		}

		//motion extraction
		if (pseqdesc->motiontype != 0)
		{
			QC_TranslateMotionFlag(pseqdesc->motiontype, flagstring, true);
			fprintf(qcfile,"%s ", flagstring);
			//fprintf(qcfile,"motiontype %i %s ",pseqdesc->motiontype, flagstring);
		}
		else
		{
			fprintf(qcfile," "); //stupid hack for spacing
		}

		//fps, loop
		fprintf(qcfile,"fps %.0f ", pseqdesc->fps);
		if (pseqdesc-> flags == STUDIO_LOOPING)
		{
			fprintf(qcfile,"loop ");
		}
		
		//AI activity, weight
		if (pseqdesc->activity != 0)
		{
			if (!QC_TranslateActivityFlag(pseqdesc->activity,flagstring))
			{
				//custom activity flag in dll code (only used by tentacle2.mdl)
				fprintf(qcfile,"ACT_%i %i ", pseqdesc->activity, pseqdesc->actweight);
			}
			else
			{
				fprintf(qcfile,"%s %i ", flagstring, pseqdesc->actweight);
			}
		}

		//transition nodes (only used by tentacle2.mdl)
		if ((pseqdesc->entrynode != 0) && (pseqdesc->exitnode != 0))
		{
			if ((pseqdesc->entrynode == pseqdesc->exitnode))
			{
				fprintf(qcfile,"node %i ", pseqdesc->entrynode );
			}
			else
			{
				if (pseqdesc->nodeflags == 0)
				{
					fprintf(qcfile,"transition %i %i ", pseqdesc->entrynode, pseqdesc->exitnode);
				}				
				else
				{
					fprintf(qcfile,"rtransition %i %i ", pseqdesc->entrynode, pseqdesc->exitnode);
				}
			}
		}

		//events
		if (pseqdesc->numevents > 2)
			fprintf(qcfile,"{\n ");

		for (j = 0; j < pseqdesc->numevents; j++) 
		{
			pevent = (mstudioevent_t *)((byte *)phdr + pseqdesc->eventindex + (sizeof(mstudioevent_t) * j));
			
			if (pseqdesc->numevents > 2)
			{
				if (pseqdesc->numblends > 2)
				{
					fprintf(qcfile,"          ");
				}
				else
				{
					fprintf(qcfile," ");
				}
			}
				
			fprintf(qcfile,"{ event %i %i", pevent->event,pevent->frame);
			if (strlen(pevent->options) != 0)
			{
				fprintf(qcfile," \"%s\"",pevent->options);
			}
			fprintf(qcfile," } ");
			if (pseqdesc->numevents > 2)
				fprintf(qcfile,"\n ");
		}
		if (pseqdesc->numevents > 2)
			fprintf(qcfile,"}");

		fprintf(qcfile,"\n");

		if (pseqdesc->numblends > 2)
			fprintf(qcfile,"}\n");

		//pivots 
		if (pseqdesc->numpivots > 0)
			LogMessage (MDLDEC_MSG_WARNING , "WARNING: Sequence %s uses %i foot pivots, feature not supported.\r\n", pseqdesc->label, pseqdesc->numpivots); 
			//printf( "WARNING: Sequence %s uses %i foot pivots, feature not supported.\n", pseqdesc->label, pseqdesc->numpivots); 

		/*
		fprintf(qcfile,"numpivots %i ", pseqdesc->numpivots);// number of foot pivots
		fprintf(qcfile,"pivotindex %i ", pseqdesc->pivotindex );		
		*/


		/*
		fprintf(qcfile,"motiontype %i ", pseqdesc->motiontype);	
		fprintf(qcfile,"motionbone %i ", pseqdesc->motionbone);
		fprintf(qcfile," automoveposindex %i ",pseqdesc->automoveposindex );
		fprintf(qcfile," automoveangleindex %i ",pseqdesc->automoveangleindex );
		fprintf(qcfile,"numblends %i ", pseqdesc->numblends );
		fprintf(qcfile,"animindex %i ", pseqdesc->animindex);					// mstudioanim_t pointer relative to start of sequence group data
		fprintf(qcfile,"blendtype1 %i ", pseqdesc->blendtype[0]);			
		fprintf(qcfile,"blendtype2 %i ", pseqdesc->blendtype[1]); // X, Y, Z, XR, YR, ZR
		fprintf(qcfile,"blendstart1 %.0f ", pseqdesc->blendstart[0]);	// starting value		
		fprintf(qcfile,"blendstart2 %.0f ", pseqdesc->blendstart[1]); 
		fprintf(qcfile,"blendend1 %.0f ",pseqdesc->blendend[0]);	// starting value		
		fprintf(qcfile,"blendend2 %.0f ", pseqdesc->blendend[1]); // ending value
		fprintf(qcfile,"blendparent %i ", pseqdesc->blendparent);	
		fprintf(qcfile,"seqgroup %i ", pseqdesc->seqgroup	);	// sequence group for demand loading
		fprintf(qcfile,"entrynode %i ",pseqdesc->entrynode );		// transition node at entry
		fprintf(qcfile,"exitnode %i ",pseqdesc->exitnode );		// transition node at exit
		fprintf(qcfile,"nodeflags %i ",pseqdesc->nodeflags );		// transition rules
		fprintf(qcfile,"nextseq %i ", pseqdesc->nextseq);
	
		fprintf(qcfile,"\n");
		*/		
	}

}


int CMDLDecompiler::QC_TranslateActivityFlag(const int aflag, char *string)
{
	strset(string,0);
	
	if (aflag == ACT_RESET) { strcpy(string,"ACT_RESET"); return true;}
	if (aflag == ACT_IDLE) { strcpy(string,"ACT_IDLE"); return true;}
	if (aflag == ACT_GUARD) { strcpy(string,"ACT_GUARD"); return true;}
	if (aflag == ACT_WALK) { strcpy(string,"ACT_WALK"); return true;}
	if (aflag == ACT_RUN) { strcpy(string,"ACT_RUN"); return true;}
	if (aflag == ACT_FLY) { strcpy(string,"ACT_FLY"); return true;}
	if (aflag == ACT_SWIM) { strcpy(string,"ACT_SWIM"); return true;}
	if (aflag == ACT_HOP) { strcpy(string,"ACT_HOP"); return true;}
	if (aflag == ACT_LEAP) { strcpy(string,"ACT_LEAP"); return true;}
	if (aflag == ACT_FALL) { strcpy(string,"ACT_FALL"); return true;}
	if (aflag == ACT_LAND) { strcpy(string,"ACT_LAND"); return true;}
	if (aflag == ACT_STRAFE_LEFT) { strcpy(string,"ACT_STRAFE_LEFT"); return true;}
	if (aflag == ACT_STRAFE_RIGHT) { strcpy(string,"ACT_STRAFE_RIGHT"); return true;}
	if (aflag == ACT_ROLL_LEFT) { strcpy(string,"ACT_ROLL_LEFT"); return true;}
	if (aflag == ACT_ROLL_RIGHT) { strcpy(string,"ACT_ROLL_RIGHT"); return true;}
	if (aflag == ACT_TURN_LEFT) { strcpy(string,"ACT_TURN_LEFT"); return true;}
	if (aflag == ACT_TURN_RIGHT) { strcpy(string,"ACT_TURN_RIGHT"); return true;}
	if (aflag == ACT_CROUCH) { strcpy(string,"ACT_CROUCH"); return true;}
	if (aflag == ACT_CROUCHIDLE) { strcpy(string,"ACT_CROUCHIDLE"); return true;}
	if (aflag == ACT_STAND) { strcpy(string,"ACT_STAND"); return true;}
	if (aflag == ACT_USE) { strcpy(string,"ACT_USE"); return true;}
	if (aflag == ACT_SIGNAL1) { strcpy(string,"ACT_SIGNAL1"); return true;}
	if (aflag == ACT_SIGNAL2) { strcpy(string,"ACT_SIGNAL2"); return true;}
	if (aflag == ACT_SIGNAL3) { strcpy(string,"ACT_SIGNAL3"); return true;}
	if (aflag == ACT_TWITCH) { strcpy(string,"ACT_TWITCH"); return true;}
	if (aflag == ACT_COWER) { strcpy(string,"ACT_COWER"); return true;}
	if (aflag == ACT_SMALL_FLINCH) { strcpy(string,"ACT_SMALL_FLINCH"); return true;}
	if (aflag == ACT_BIG_FLINCH) { strcpy(string,"ACT_BIG_FLINCH"); return true;}
	if (aflag == ACT_RANGE_ATTACK1) { strcpy(string,"ACT_RANGE_ATTACK1"); return true;}
	if (aflag == ACT_RANGE_ATTACK2) { strcpy(string,"ACT_RANGE_ATTACK2"); return true;}
	if (aflag == ACT_MELEE_ATTACK1) { strcpy(string,"ACT_MELEE_ATTACK1"); return true;}
	if (aflag == ACT_MELEE_ATTACK2) { strcpy(string,"ACT_MELEE_ATTACK2"); return true;}
	if (aflag == ACT_RELOAD) { strcpy(string,"ACT_RELOAD"); return true;}
	if (aflag == ACT_ARM) { strcpy(string,"ACT_ARM"); return true;}
	if (aflag == ACT_DISARM) { strcpy(string,"ACT_DISARM"); return true;}
	if (aflag == ACT_EAT) { strcpy(string,"ACT_EAT"); return true;}
	if (aflag == ACT_DIESIMPLE) { strcpy(string,"ACT_DIESIMPLE"); return true;}
	if (aflag == ACT_DIEBACKWARD) { strcpy(string,"ACT_DIEBACKWARD"); return true;}
	if (aflag == ACT_DIEFORWARD) { strcpy(string,"ACT_DIEFORWARD"); return true;}
	if (aflag == ACT_DIEVIOLENT) { strcpy(string,"ACT_DIEVIOLENT"); return true;}
	if (aflag == ACT_BARNACLE_HIT) { strcpy(string,"ACT_BARNACLE_HIT"); return true;}
	if (aflag == ACT_BARNACLE_PULL) { strcpy(string,"ACT_BARNACLE_PULL"); return true;}
	if (aflag == ACT_BARNACLE_CHOMP) { strcpy(string,"ACT_BARNACLE_CHOMP"); return true;}
	if (aflag == ACT_BARNACLE_CHEW) { strcpy(string,"ACT_BARNACLE_CHEW"); return true;}
	if (aflag == ACT_SLEEP) { strcpy(string,"ACT_SLEEP"); return true;}
	if (aflag == ACT_INSPECT_FLOOR) { strcpy(string,"ACT_INSPECT_FLOOR"); return true;}
	if (aflag == ACT_INSPECT_WALL) { strcpy(string,"ACT_INSPECT_WALL"); return true;}
	if (aflag == ACT_IDLE_ANGRY) { strcpy(string,"ACT_IDLE_ANGRY"); return true;}
	if (aflag == ACT_WALK_HURT) { strcpy(string,"ACT_WALK_HURT"); return true;}
	if (aflag == ACT_RUN_HURT) { strcpy(string,"ACT_RUN_HURT"); return true;}
	if (aflag == ACT_HOVER) { strcpy(string,"ACT_HOVER"); return true;}
	if (aflag == ACT_GLIDE) { strcpy(string,"ACT_GLIDE"); return true;}
	if (aflag == ACT_FLY_LEFT) { strcpy(string,"ACT_FLY_LEFT"); return true;}
	if (aflag == ACT_FLY_RIGHT) { strcpy(string,"ACT_FLY_RIGHT"); return true;}
	if (aflag == ACT_DETECT_SCENT) { strcpy(string,"ACT_DETECT_SCENT"); return true;}
	if (aflag == ACT_SNIFF) { strcpy(string,"ACT_SNIFF"); return true;}
	if (aflag == ACT_BITE) { strcpy(string,"ACT_BITE"); return true;}
	if (aflag == ACT_THREAT_DISPLAY) { strcpy(string,"ACT_THREAT_DISPLAY"); return true;}
	if (aflag == ACT_FEAR_DISPLAY) { strcpy(string,"ACT_FEAR_DISPLAY"); return true;}
	if (aflag == ACT_EXCITED) { strcpy(string,"ACT_EXCITED"); return true;}
	if (aflag == ACT_SPECIAL_ATTACK1) { strcpy(string,"ACT_SPECIAL_ATTACK1"); return true;}
	if (aflag == ACT_SPECIAL_ATTACK2) { strcpy(string,"ACT_SPECIAL_ATTACK2"); return true;}
	if (aflag == ACT_COMBAT_IDLE) { strcpy(string,"ACT_COMBAT_IDLE"); return true;}
	if (aflag == ACT_WALK_SCARED) { strcpy(string,"ACT_WALK_SCARED"); return true;}
	if (aflag == ACT_RUN_SCARED) { strcpy(string,"ACT_RUN_SCARED"); return true;}
	if (aflag == ACT_VICTORY_DANCE) { strcpy(string,"ACT_VICTORY_DANCE"); return true;}
	if (aflag == ACT_DIE_HEADSHOT) { strcpy(string,"ACT_DIE_HEADSHOT"); return true;}
	if (aflag == ACT_DIE_CHESTSHOT) { strcpy(string,"ACT_DIE_CHESTSHOT"); return true;}
	if (aflag == ACT_DIE_GUTSHOT) { strcpy(string,"ACT_DIE_GUTSHOT"); return true;}
	if (aflag == ACT_DIE_BACKSHOT) { strcpy(string,"ACT_DIE_BACKSHOT"); return true;}
	if (aflag == ACT_FLINCH_HEAD) { strcpy(string,"ACT_FLINCH_HEAD"); return true;}
	if (aflag == ACT_FLINCH_CHEST) { strcpy(string,"ACT_FLINCH_CHEST"); return true;}
	if (aflag == ACT_FLINCH_STOMACH) { strcpy(string,"ACT_FLINCH_STOMACH"); return true;}
	if (aflag == ACT_FLINCH_LEFTARM) { strcpy(string,"ACT_FLINCH_LEFTARM"); return true;}
	if (aflag == ACT_FLINCH_RIGHTARM) { strcpy(string,"ACT_FLINCH_RIGHTARM"); return true;}
	if (aflag == ACT_FLINCH_LEFTLEG) { strcpy(string,"ACT_FLINCH_LEFTLEG"); return true;}
	if (aflag == ACT_FLINCH_RIGHTLEG) { strcpy(string,"ACT_FLINCH_RIGHTLEG"); return true;}
	return false;
}


void CMDLDecompiler::QC_TranslateMotionFlag(const int mflag, char *string, bool addtostring)
{
	strset(string,0);

	if ( !addtostring )
	{
		switch( mflag & STUDIO_TYPES )
		{
			case STUDIO_X: strcpy(string,"X"); break;
			case STUDIO_Y: strcpy(string,"Y"); break;
			case STUDIO_Z: strcpy(string,"Z"); break;
			case STUDIO_XR: strcpy(string,"XR"); break;
			case STUDIO_YR: strcpy(string,"YR"); break;
			case STUDIO_ZR: strcpy(string,"ZR"); break;
			case STUDIO_LX: strcpy(string,"LX"); break;
			case STUDIO_LY: strcpy(string,"LY"); break;
			case STUDIO_LZ: strcpy(string,"LZ"); break;
			case STUDIO_AX: strcpy(string,"AX"); break;
			case STUDIO_AY: strcpy(string,"AY"); break;
			case STUDIO_AZ: strcpy(string,"AZ"); break;
			case STUDIO_AXR: strcpy(string,"AXR"); break;
			case STUDIO_AYR: strcpy(string,"AYR"); break;
			case STUDIO_AZR: strcpy(string,"AZR"); break;
			//case STUDIO_RLOOP: strcpy(string,"loop"); break;
		}
	}
	else
	{
		if (mflag & STUDIO_X) {strcat(string," X");}
		if (mflag & STUDIO_Y) {strcat(string," Y");}
		if (mflag & STUDIO_Z) {strcat(string," Z");}
		if (mflag & STUDIO_XR) {strcat(string," XR");}
		if (mflag & STUDIO_YR) {strcat(string," YR");}
		if (mflag & STUDIO_ZR) {strcat(string," ZR");}
		if (mflag & STUDIO_LX) {strcat(string," LX");}
		if (mflag & STUDIO_LY) {strcat(string," LY");}
		if (mflag & STUDIO_LZ) {strcat(string," LZ");}
		if (mflag & STUDIO_AX) {strcat(string," AX");}
		if (mflag & STUDIO_AY) {strcat(string," AY");}
		if (mflag & STUDIO_AZ) {strcat(string," AZ");}
		if (mflag & STUDIO_AXR) {strcat(string," AXR");}
		if (mflag & STUDIO_AYR) {strcat(string," AYR");}
		if (mflag & STUDIO_AZR) {strcat(string," AZR");}
		//printf("addtostring %i mflag %i string %s\n",addtostring, mflag,string);
	}

}
