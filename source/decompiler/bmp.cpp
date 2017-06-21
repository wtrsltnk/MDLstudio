/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/


#include "mdldec.h"


void CMDLDecompiler::BMP_GenerateTextures()
{

	byte			*pColorData;
	byte			*pBitData;
	studiohdr_t		*pHdr;
	mstudiotexture_t *pTexture;
	int				i;

	pHdr=(studiohdr_t *)CoreFile;

	if(pHdr->numtextures==0)
	{
		pHdr=(studiohdr_t *)TextureFile; // textures are stored in a separated t.mdl/dol file
	}

	for (i = 0; i < pHdr->numtextures; i++) //loop textures
	{
		pTexture=(mstudiotexture_t *)((byte *)pHdr + pHdr->textureindex + sizeof(mstudiotexture_t) * i);
		pBitData=(byte *)pHdr + pTexture->index; 
		pColorData=(byte *)pHdr + pTexture->index + (pTexture->width * pTexture->height);
		BMP_WriteTexture(pBitData, pColorData, pTexture->name, pTexture); //write texture
	}	
		
}

void CMDLDecompiler::BMP_WriteTexture(byte *pBitData, byte *pColorData, char *TextureName, mstudiotexture_t *pTexture)
{
	
	FILE				*fp;
	char				Filename[_MAX_PATH];
	BITMAPFILEHEADER	bmfh;
	BITMAPINFOHEADER	bmih;
	RGBQUAD				rgbPalette[256];
	ULONG				cbBmpBits;
	BYTE*				pbBmpBits;
	byte				*pb, *pbPal = NULL;
	ULONG				cbPalBytes;
	ULONG				biTrueWidth;
	int					i;
	
	char tmpname[64];
	MyExtractFileBase( TextureName, tmpname );
	sprintf(Filename, "%s%s", DestPath, TextureName);

	if((fp=fopen(Filename,"wb"))==NULL)
	{
		LogMessage (MDLDEC_MSG_ERROR , "ERROR: Couln't write texture file %s\r\n",Filename);
		//printf ("\nERROR: Couln't write texture file %s\n",Filename);
		return;
	}

	//biTrueWidth = ((pTexture->width + 3) & ~3); //Uh? What the hell does this...
	biTrueWidth = pTexture->width; 

	cbBmpBits = biTrueWidth * pTexture->height;
	cbPalBytes = 256 * sizeof( RGBQUAD );

	//This creates a bmp that can be used by studiomdl

	//BITMAPFILEHEADER
	bmfh.bfType = MAKEWORD( 'B', 'M' );
	bmfh.bfSize = sizeof bmfh + sizeof bmih + cbBmpBits + cbPalBytes;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof bmfh + sizeof bmih + cbPalBytes;
	// Write file header
	fwrite(&bmfh, sizeof bmfh, 1, fp);

	//BITMAPINFOHEADER
	bmih.biSize = sizeof bmih;
	bmih.biWidth = biTrueWidth;
	bmih.biHeight = pTexture->height;
	bmih.biPlanes = 1;						
	bmih.biBitCount = 8;					
	bmih.biCompression = BI_RGB;			
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;				
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 256;					
	bmih.biClrImportant = 0;
	// Write info header
	fwrite(&bmih, sizeof bmih, 1, fp);		

	pb = pColorData;

	// Copy over used entries
	for (i = 0; i < (int)bmih.biClrUsed; i++)
	{
		rgbPalette[i].rgbRed   = *pb++;
		rgbPalette[i].rgbGreen = *pb++;
		rgbPalette[i].rgbBlue  = *pb++;
        rgbPalette[i].rgbReserved = 0; 
	}

	// Write palette (bmih.biClrUsed entries)
	cbPalBytes = bmih.biClrUsed * sizeof( RGBQUAD );
	fwrite(rgbPalette, cbPalBytes, 1, fp);
	pbBmpBits = (byte *)malloc(cbBmpBits*sizeof(byte));
	pb = pBitData;
	// reverse the order of the data.
	pb += (pTexture->height - 1) * pTexture->width;
	for(i = 0; i < bmih.biHeight; i++)
	{
		memmove(&pbBmpBits[biTrueWidth * i], pb, pTexture->width);
		pb -= pTexture->width;
	}
	// Write bitmap bits (remainder of file)
	fwrite(pbBmpBits, cbBmpBits, 1, fp);
	free(pbBmpBits);
	fclose(fp); // done
	LogMessage (MDLDEC_MSG_INFO , "Texture: %s\r\n", Filename);
	//printf( "Texture: %s\n", Filename);
}


