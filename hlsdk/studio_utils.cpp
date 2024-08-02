/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
****/
// updates:
// 1-4-99	fixed file texture load and file read bug
// 2-8-99	fixed demand loaded sequence bug (thanks to Frans 'Otis' Bouma)

////////////////////////////////////////////////////////////////////////
#include <stdio.h>

#include <windows.h>

#include <glad/glad.h>

#include "mathlib.h"
//#include "..\..\engine\studio.h"
#include "studio.h"
#include "studio_render.h"

#include "..\resource\resource.h"
#pragma warning(disable : 4244) // double to float

////////////////////////////////////////////////////////////////////////

static int g_texnum = 0;

void StudioModel::UploadTexture(
    mstudiotexture_t *ptexture,
    byte *data,
    byte *pal,
    int texnum,
    bool import)
{
    // unsigned *in, int inwidth, int inheight, unsigned *out,  int outwidth, int outheight;
    int i, j;
    int row1[256], row2[256], col1[256], col2[256];
    byte *pix1, *pix2, *pix3, *pix4;
    byte *tex, *out;

    // convert texture to power of 2
    int outwidth = 1;
    for (; outwidth < ptexture->width; outwidth <<= 1)
        ;

    if (outwidth > 256)
        outwidth = 256;

    int outheight = 1;
    for (; outheight < ptexture->height; outheight <<= 1)
        ;

    if (outheight > 256)
        outheight = 256;

    tex = out = (byte *)malloc(outwidth * outheight * 4);

    for (i = 0; i < outwidth; i++)
    {
        col1[i] = (i + 0.25) * (ptexture->width / (float)outwidth);
        col2[i] = (i + 0.75) * (ptexture->width / (float)outwidth);
    }

    for (i = 0; i < outheight; i++)
    {
        row1[i] = (int)((i + 0.25) * (ptexture->height / (float)outheight)) * ptexture->width;
        row2[i] = (int)((i + 0.75) * (ptexture->height / (float)outheight)) * ptexture->width;
    }

    // scale down and convert to 32bit RGB
    for (i = 0; i < outheight; i++)
    {
        for (j = 0; j < outwidth; j++, out += 4)
        {
            pix1 = &pal[data[row1[i] + col1[j]] * 3];
            pix2 = &pal[data[row1[i] + col2[j]] * 3];
            pix3 = &pal[data[row2[i] + col1[j]] * 3];
            pix4 = &pal[data[row2[i] + col2[j]] * 3];

            out[0] = (pix1[0] + pix2[0] + pix3[0] + pix4[0]) >> 2;
            out[1] = (pix1[1] + pix2[1] + pix3[1] + pix4[1]) >> 2;
            out[2] = (pix1[2] + pix2[2] + pix3[2] + pix4[2]) >> 2;
            if ((out[0] == 0) && (out[1] == 0) && (out[2] == 255)) // Is Pixel Completely Black
                out[3] = 0;                                        // Set The Alpha Value To 0
            else                                                   // Otherwise
                out[3] = 255;
            //out[3] = 0xFF;
        }
    }

    glBindTexture(GL_TEXTURE_2D, texnum);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, outwidth, outheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ptexture->width = outwidth;
    // ptexture->height = outheight;
    ptexture->index = texnum;

    import ? g_texnum : g_texnum++;
    free(tex);
}

void StudioModel::FreeModel()
{
    if (pSavedTIndexes)
        free(pSavedTIndexes);

    if (m_pstudiohdr)
        free(m_pstudiohdr);

    if (m_ptexturehdr && m_owntexmodel)
        free(m_ptexturehdr);

    m_pstudiohdr = m_ptexturehdr = 0;
    m_owntexmodel = false;

    for (int i = 0; i < 32; i++)
    {
        if (m_panimhdr[i])
        {
            free(m_panimhdr[i]);
            m_panimhdr[i] = 0;
        }
    }

    // deleting textures
    int textures[MAXSTUDIOSKINS];
    for (int i = 0; i < g_texnum; i++)
    {
        textures[i] = i;
    }

    glDeleteTextures(g_texnum, (const GLuint *)textures);

    g_texnum = 0;
}

bool StudioModel::LoadModel(
    const char *modelname)
{
    FILE *fp;

    errno_t err = fopen_s(&fp, modelname, "rb");

    // load the model
    if (err != 0)
    {
        char str[MAX_PATH + 22];
        sprintf_s(str, MAX_PATH + 22, "unable to open %s\n", modelname);

        return false;
    }

    fseek(fp, 0, SEEK_END);
    CoreFileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    CoreFile = (byte *)malloc(CoreFileLength);
    fread(CoreFile, CoreFileLength, 1, fp);

    byte *pin;
    studiohdr_t *phdr;
    mstudiotexture_t *ptexture;

    pin = (byte *)CoreFile;
    phdr = (studiohdr_t *)pin;

    ptexture = (mstudiotexture_t *)(pin + phdr->textureindex);
    if (phdr->textureindex > 0)
    {
        pSavedTIndexes = (int *)malloc(phdr->numtextures * sizeof(int));
        if (pSavedTIndexes)
        {
            for (int i = 0; i < phdr->numtextures; i++)
            {
                // strcpy( name, mod->name );
                // strcpy( name, ptexture[i].name );
                pSavedTIndexes[i] = ptexture[i].index;
                UploadTexture(&ptexture[i], pin + ptexture[i].index, pin + ptexture[i].width * ptexture[i].height + ptexture[i].index, i, false);
            }
        }
        else
        {
            char str[MAX_PATH + 22];
            sprintf_s(str, MAX_PATH + 22, "error on openening %s\n", modelname);

            fclose(fp);

            return false;
        }
    }

    // UNDONE: free texture memory

    fclose(fp);

    return true;
}

bool StudioModel::SaveModel(
    const char *modelname)
{
    FILE *fp;
    studiohdr_t *pHdr;

    errno_t err = fopen_s(&fp, modelname, "wb");

    // open the model
    if (err != 0)
    {
        char str[256];
        sprintf_s(str, 256, "unable to save %s\n", modelname);

        return false;
    }

    pHdr = (studiohdr_t *)CoreFile;
    if (pHdr->numtextures != 0)
    {
        studiohdr_t *pTmpHdr;
        byte *pTmpFile;
        mstudiotexture_t *pTextures;

        // there are textures in this datablock. copy filedata to tempbuffer, reset texture
        // indices and save that block
        pTmpFile = (byte *)malloc(CoreFileLength);
        // copy data...
        for (int i = 0; i < CoreFileLength; i++)
        {
            pTmpFile[i] = CoreFile[i];
        }
        // reset indices.
        pTmpHdr = (studiohdr_t *)pTmpFile;
        pTextures = (mstudiotexture_t *)((byte *)pTmpHdr + pTmpHdr->textureindex);
        for (int i = 0; i < pHdr->numtextures; i++)
        {
            pTextures[i].index = pSavedTIndexes[i];
        }
        // save this block
        fwrite(pTmpFile, CoreFileLength, 1, fp);
        // free mem
        free(pTmpFile);
        fclose(fp);
    }
    else
    {
        // No textures here: save the corefile data...
        fwrite(CoreFile, CoreFileLength, 1, fp);
        // done
        fclose(fp);
    }

    return true;
}

bool StudioModel::LoadTextures(
    const char *modelname)
{
    FILE *fp;

    errno_t err = fopen_s(&fp, modelname, "rb");

    // load the model
    if (err != 0)
    {
        char str[256];
        sprintf_s(str, 256, "unable to open %s\n", modelname);

        return false;
    }

    fseek(fp, 0, SEEK_END);
    TextureFileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    TextureFile = (byte *)malloc(TextureFileLength);
    fread(TextureFile, TextureFileLength, 1, fp);

    byte *pin;
    studiohdr_t *phdr;
    mstudiotexture_t *ptexture;

    pin = (byte *)TextureFile;
    phdr = (studiohdr_t *)pin;

    ptexture = (mstudiotexture_t *)(pin + phdr->textureindex);
    if (phdr->textureindex != 0)
    {
        for (int i = 0; i < phdr->numtextures; i++)
        {
            // strcpy( name, mod->name );
            // strcpy( name, ptexture[i].name );
            UploadTexture(&ptexture[i], pin + ptexture[i].index, pin + ptexture[i].width * ptexture[i].height + ptexture[i].index, i, false);
        }
    }

    // UNDONE: free texture memory

    fclose(fp);

    return true;
}

bool StudioModel::LoadDemandSequence(
    const char *modelname,
    int index)
{
    FILE *fp;

    errno_t err = fopen_s(&fp, modelname, "rb");

    // load the model
    if (err != 0)
    {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    SequenceFilesLength[index] = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    SequenceFiles[index] = (byte *)malloc(SequenceFilesLength[index]);
    fread(SequenceFiles[index], SequenceFilesLength[index], 1, fp);

    fclose(fp);

    return true;
}

bool StudioModel::Init(
    const char *modelname)
{
    if (!LoadModel(modelname))
    {
        return false;
    }

    m_pstudiohdr = (studiohdr_t *)CoreFile;

    // test for the IDST tag...
    if (memcmp(CoreFile, "IDST", 4) != 0)
    {
        free(CoreFile);
        CoreFileLength = 0;

        return false;
    }

    if (!m_pstudiohdr)
    {
        return false;
    }

    // preload textures
    if (m_pstudiohdr->numtextures == 0)
    {
        char texturename[256];

        strcpy_s(texturename, 256, modelname);
        strcpy_s(&texturename[strlen(texturename) - 4], 256 - (strlen(texturename) - 4), "T.mdl");

        if (!LoadModel(texturename))
        {
            return false;
        }

        m_ptexturehdr = (studiohdr_t *)CoreFile;

        if (!m_ptexturehdr)
        {
            return false;
        }
    }
    else
    {
        m_ptexturehdr = m_pstudiohdr;
    }

    // preload animations
    if (m_pstudiohdr->numseqgroups > 1)
    {
        for (int i = 1; i < m_pstudiohdr->numseqgroups; i++)
        {
            char seqgroupname[256];

            strcpy_s(seqgroupname, 256, modelname);
            sprintf_s(&seqgroupname[strlen(seqgroupname) - 4], 256 - (strlen(seqgroupname) - 4), "%02d.mdl", i);

            if (LoadDemandSequence(seqgroupname, i))
            {
                return false;
            }

            m_panimhdr[i] = (studioseqhdr_t *)SequenceFiles[i];

            if (!m_panimhdr[i])
            {
                return false;
            }
        }
    }

    return true;
}
////////////////////////////////////////////////////////////////////////

int StudioModel::GetSequence()
{
    return m_sequence;
}

mstudioseqdesc_t *StudioModel::GetSequence(
    const char *name)
{
    mstudioseqdesc_t *pseqdescs = (mstudioseqdesc_t *)((byte *)m_pstudiohdr + m_pstudiohdr->seqindex);
    for (int i = 0; i < m_pstudiohdr->numseq; i++)
    {
        if (strcmp(pseqdescs[i].label, name) == 0)
        {
            return &pseqdescs[i];
        }
    }
    return NULL;
}
mstudioevent_t *StudioModel::GetEvent(
    const char *seqname)
{
    mstudioseqdesc_t *pseqdescs = (mstudioseqdesc_t *)((byte *)m_pstudiohdr + m_pstudiohdr->seqindex);
    for (int i = 0; i < m_pstudiohdr->numseq; i++)
    {
        if (strcmp(pseqdescs[i].label, seqname) == 0)
        {
            return (mstudioevent_t *)((byte *)m_pstudiohdr + pseqdescs[i].eventindex);
        }
    }
    return NULL;
}

int StudioModel::SetSequence(
    int iSequence)
{
    if (iSequence > m_pstudiohdr->numseq)
        iSequence = 0;
    if (iSequence < 0)
        iSequence = m_pstudiohdr->numseq - 1;

    m_sequence = iSequence;
    m_frame = 0;

    return m_sequence;
}

float StudioModel::GetFrame()
{
    return m_frame;
}

void StudioModel::SetFrame(
    int frame)
{
    m_frame = (float)frame;
}

void StudioModel::ExtractBbox(
    float *mins,
    float *maxs)
{
    mstudioseqdesc_t *pseqdesc;

    pseqdesc = (mstudioseqdesc_t *)((byte *)m_pstudiohdr + m_pstudiohdr->seqindex);

    mins[0] = pseqdesc[m_sequence].bbmin[0];
    mins[1] = pseqdesc[m_sequence].bbmin[1];
    mins[2] = pseqdesc[m_sequence].bbmin[2];

    maxs[0] = pseqdesc[m_sequence].bbmax[0];
    maxs[1] = pseqdesc[m_sequence].bbmax[1];
    maxs[2] = pseqdesc[m_sequence].bbmax[2];
}

void StudioModel::GetSequenceInfo(
    float *pflFrameRate,
    float *pflGroundSpeed)
{
    mstudioseqdesc_t *pseqdesc;

    pseqdesc = (mstudioseqdesc_t *)((byte *)m_pstudiohdr + m_pstudiohdr->seqindex) + (int)m_sequence;

    if (pseqdesc->numframes > 1)
    {
        *pflFrameRate = 256 * pseqdesc->fps / (pseqdesc->numframes - 1);
        *pflGroundSpeed = sqrt(pseqdesc->linearmovement[0] * pseqdesc->linearmovement[0] + pseqdesc->linearmovement[1] * pseqdesc->linearmovement[1] + pseqdesc->linearmovement[2] * pseqdesc->linearmovement[2]);
        *pflGroundSpeed = *pflGroundSpeed * pseqdesc->fps / (pseqdesc->numframes - 1);
    }
    else
    {
        *pflFrameRate = 256.0;
        *pflGroundSpeed = 0.0;
    }
}

float StudioModel::SetController(
    int iController,
    float flValue)
{
    mstudiobonecontroller_t *pbonecontroller = (mstudiobonecontroller_t *)((byte *)m_pstudiohdr + m_pstudiohdr->bonecontrollerindex);

    // find first controller that matches the index
    int i = 0;
    for (; i < m_pstudiohdr->numbonecontrollers; i++, pbonecontroller++)
    {
        if (pbonecontroller->index == iController)
            break;
    }
    if (i >= m_pstudiohdr->numbonecontrollers)
        return flValue;

    // wrap 0..360 if it's a rotational controller
    if (pbonecontroller->type & (STUDIO_XR | STUDIO_YR | STUDIO_ZR))
    {
        // ugly hack, invert value if end < start
        if (pbonecontroller->end < pbonecontroller->start)
            flValue = -flValue;

        // does the controller not wrap?
        if (pbonecontroller->start + 359.0 >= pbonecontroller->end)
        {
            if (flValue > ((pbonecontroller->start + pbonecontroller->end) / 2.0) + 180)
                flValue = flValue - 360;
            if (flValue < ((pbonecontroller->start + pbonecontroller->end) / 2.0) - 180)
                flValue = flValue + 360;
        }
        else
        {
            if (flValue > 360)
                flValue = flValue - (int)(flValue / 360.0) * 360.0;
            else if (flValue < 0)
                flValue = flValue + (int)((flValue / -360.0) + 1) * 360.0;
        }
    }

    int setting = 255 * (flValue - pbonecontroller->start) / (pbonecontroller->end - pbonecontroller->start);

    if (setting < 0) setting = 0;
    if (setting > 255) setting = 255;
    m_controller[iController] = setting;

    return setting * (1.0 / 255.0) * (pbonecontroller->end - pbonecontroller->start) + pbonecontroller->start;
}

float StudioModel::SetMouth(
    float flValue)
{
    mstudiobonecontroller_t *pbonecontroller = (mstudiobonecontroller_t *)((byte *)m_pstudiohdr + m_pstudiohdr->bonecontrollerindex);

    // find first controller that matches the mouth
    for (int i = 0; i < m_pstudiohdr->numbonecontrollers; i++, pbonecontroller++)
    {
        if (pbonecontroller->index == 4)
            break;
    }

    // wrap 0..360 if it's a rotational controller
    if (pbonecontroller->type & (STUDIO_XR | STUDIO_YR | STUDIO_ZR))
    {
        // ugly hack, invert value if end < start
        if (pbonecontroller->end < pbonecontroller->start)
            flValue = -flValue;

        // does the controller not wrap?
        if (pbonecontroller->start + 359.0 >= pbonecontroller->end)
        {
            if (flValue > ((pbonecontroller->start + pbonecontroller->end) / 2.0) + 180)
                flValue = flValue - 360;
            if (flValue < ((pbonecontroller->start + pbonecontroller->end) / 2.0) - 180)
                flValue = flValue + 360;
        }
        else
        {
            if (flValue > 360)
                flValue = flValue - (int)(flValue / 360.0) * 360.0;
            else if (flValue < 0)
                flValue = flValue + (int)((flValue / -360.0) + 1) * 360.0;
        }
    }

    int setting = 0;
    if ((pbonecontroller->end - pbonecontroller->start) != 0)
        setting = 64 * (flValue - pbonecontroller->start) / (pbonecontroller->end - pbonecontroller->start);

    if (setting < 0) setting = 0;
    if (setting > 64) setting = 64;
    m_mouth = setting;

    return setting * (1.0 / 64.0) * (pbonecontroller->end - pbonecontroller->start) + pbonecontroller->start;
}

float StudioModel::SetBlending(
    int iBlender,
    float flValue)
{
    mstudioseqdesc_t *pseqdesc;

    pseqdesc = (mstudioseqdesc_t *)((byte *)m_pstudiohdr + m_pstudiohdr->seqindex) + (int)m_sequence;

    if (pseqdesc->blendtype[iBlender] == 0)
        return flValue;

    if (pseqdesc->blendtype[iBlender] & (STUDIO_XR | STUDIO_YR | STUDIO_ZR))
    {
        // ugly hack, invert value if end < start
        if (pseqdesc->blendend[iBlender] < pseqdesc->blendstart[iBlender])
            flValue = -flValue;

        // does the controller not wrap?
        if (pseqdesc->blendstart[iBlender] + 359.0 >= pseqdesc->blendend[iBlender])
        {
            if (flValue > ((pseqdesc->blendstart[iBlender] + pseqdesc->blendend[iBlender]) / 2.0) + 180)
                flValue = flValue - 360;
            if (flValue < ((pseqdesc->blendstart[iBlender] + pseqdesc->blendend[iBlender]) / 2.0) - 180)
                flValue = flValue + 360;
        }
    }

    int setting = 255 * (flValue - pseqdesc->blendstart[iBlender]) / (pseqdesc->blendend[iBlender] - pseqdesc->blendstart[iBlender]);

    if (setting < 0) setting = 0;
    if (setting > 255) setting = 255;

    m_blending[iBlender] = setting;

    return setting * (1.0 / 255.0) * (pseqdesc->blendend[iBlender] - pseqdesc->blendstart[iBlender]) + pseqdesc->blendstart[iBlender];
}

int StudioModel::SetBodygroup(
    int iGroup,
    int iValue)
{
    if (iGroup > m_pstudiohdr->numbodyparts)
        return -1;

    mstudiobodyparts_t *pbodypart = (mstudiobodyparts_t *)((byte *)m_pstudiohdr + m_pstudiohdr->bodypartindex) + iGroup;

    int iCurrent = (m_bodynum / pbodypart->base) % pbodypart->nummodels;

    if (iValue >= pbodypart->nummodels)
        return iCurrent;

    m_bodynum = (m_bodynum - (iCurrent * pbodypart->base) + (iValue * pbodypart->base));

    return iValue;
}

int StudioModel::SetSkin(
    int iValue)
{
    if (iValue > m_pstudiohdr->numskinfamilies)
    {
        return m_skinnum;
    }

    m_skinnum = iValue;

    return iValue;
}

void StudioModel::SetSelHitbox(
    int index)
{
    m_selhitbox = index;
}

void StudioModel::SetSelBone(
    int index)
{
    m_selbone = index;
}
