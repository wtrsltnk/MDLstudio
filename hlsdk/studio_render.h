/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
****/
#ifndef STUDIOMODEL_H
#define STUDIOMODEL_H
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char 		byte;

class StudioModel
{
public:
	studiohdr_t				*getStudioHeader () const { return m_pstudiohdr; }
	mstudiomodel_t			*getStudioModel () const { return m_pmodel; }
	studiohdr_t				*getTextureHeader () const { return m_ptexturehdr; }
	studioseqhdr_t			*getAnimHeader (int i) const { return m_panimhdr[i]; }

	bool					Init( char *modelname ) throw();
	bool					SaveModel( char *modelname ) throw();
	void					ExportCurrentTexture();
	void					ImportCurrentTexture();
	void					DrawModel( void );
	void					DrawIcosahedron(float x,float y,float z,float size);
	void					DrawBones( void );
	void					DrawHitboxes( void );
	void					DrawAtachments( void );
	void					DrawMaterial( int skin, int nWidth, int nHeight );
	void					DrawUVMap( int skin, int nWidth, int nHeight );
	float					AdvanceFrame( float dt );

	void					FreeModel ();
	void					ExtractBbox( float *mins, float *maxs );

	int						SetSequence( int iSequence );
	int						GetSequence( void );
	mstudioseqdesc_t		*GetSequence( char *name );
	mstudioevent_t			*GetEvent( char *seqname );
	void					GetSequenceInfo( float *pflFrameRate, float *pflGroundSpeed );
	float					GetFrame( void );
	void					SetFrame( int frame );

	float					SetController( int iController, float flValue );
	float					SetMouth( float flValue );
	float					SetBlending( int iBlender, float flValue );
	int						SetBodygroup( int iGroup, int iValue );
	int						SetSkin( int iValue );

	void					SetSelHitbox( int index );
	void					SetSelBone( int index );
	long					GetPolyCount();
	mstudiotexture_t		*GetMaterial( char *name );

// EXPORT FUNCTIES
	bool					BMP_ExportTexture(char *Texname, char *Filename);
	bool					BMP_ImportTexture(char *Texname, char *Filename);
	bool					BMP_WriteTexture(byte *pBitData, byte *pColorData, char *sFilename, mstudiotexture_t *pTexture);
	bool					BMP_ReadTexture(byte *pBitData, byte *pColorData, char *sFilename, mstudiotexture_t *pTexture);
	bool					SMD_ExportSequence(char *Seqname, char *Filename);
	void					SMD_WriteFrame(FILE *smdfile, int frame, mstudioanim_t *panim, mstudioseqdesc_t *pseqdesc);
	void					SMD_WriteNodes(FILE *smd);
	bool					SMD_GenerateReferences(int Bodyindex, char *Refname, char *Filename);
	void					SMD_WriteTriangle(FILE *smdfile, mstudiomodel_t *pmodel, mstudiotexture_t *ptexture, short *pA, short *pB, short *pC);
private:
	// entity settings
	vec3_t					m_origin;
	vec3_t					m_angles;	
	int						m_sequence;			// sequence index
	float					m_frame;			// frame
	int						m_bodynum;			// bodypart selection	
	int						m_skinnum;			// skin group selection
	byte					m_controller[4];	// bone controllers
	byte					m_blending[2];		// animation blending
	byte					m_mouth;			// mouth position
	bool					m_owntexmodel;		// do we have a modelT.mdl ?
	int						m_selhitbox;
	int						m_selbone;
	long					m_polycount;
	float					g_bonetransform[MAXSTUDIOBONES][3][4];	// bone transformation matrix

	byte					*CoreFile;
	int						CoreFileLength;

	byte					*SequenceFiles[32];
	int						SequenceFilesLength[32];

	byte					*TextureFile;
	int						TextureFileLength;

	int						*pSavedTIndexes;	// pointer to buffer where saved textureindexes are put for modelsave...
	
	// internal data
	studiohdr_t				*m_pstudiohdr;
	mstudiomodel_t			*m_pmodel;

	studiohdr_t				*m_ptexturehdr;
	studioseqhdr_t			*m_panimhdr[32];

	vec4_t					m_adj;				// FIX: non persistant, make static

	bool					LoadModel( char *modelname ) throw();
	bool					LoadTextures( char *modelname ) throw();
	bool					LoadDemandSequence( char *modelname, int index) throw();

	void					CalcBoneAdj( void );
	void					CalcBoneQuaternion( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *q );
	void					CalcBonePosition( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *pos );
	void					CalcRotations ( vec3_t *pos, vec4_t *q, mstudioseqdesc_t *pseqdesc, mstudioanim_t *panim, float f );
	mstudioanim_t			*GetAnim( mstudioseqdesc_t *pseqdesc );
	void					SlerpBones( vec4_t q1[], vec3_t pos1[], vec4_t q2[], vec3_t pos2[], float s );
	void					SetUpBones ( void );

	void					DrawPoints( void );

	void					Lighting (float *lv, int bone, int flags, vec3_t normal);
	void					Chrome (int *chrome, int bone, vec3_t normal);

	void					SetupLighting( void );

	void					SetupModel ( int bodypart );

	void					UploadTexture( mstudiotexture_t *ptexture, byte *data, byte *pal, int texnum, bool import );
};

extern vec3_t g_vright;		// needs to be set to viewer's right in order for chrome to work
extern float g_lambert;		// modifier for pseudo-hemispherical lighting

#endif