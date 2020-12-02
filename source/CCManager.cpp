#include "CCManager.h"

/////////////////////////////////////
// Constructors / Destructors      //
/////////////////////////////////////
CCManager::CCManager(
    HINSTANCE hInstance,
    HWND hParent)
{
    HFONT hFont;
    this->m_hInstance = hInstance;
    this->m_hParent = hParent;
    this->dwLastTime = GetTickCount();
    this->m_nView = VIEW_MODEL;

    m_pPanel = new CTreeView(m_hInstance, m_hParent, 0, 0, 10, 10, ID_PANEL);
    m_pPanel->create(WS_EX_STATICEDGE, TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS);

    hFont = CreateFont(10, 4, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, NULL, "Fixed days");
    m_pHide = new CButton(m_hInstance, m_hParent, 0, 0, 10, 10, ID_HIDE);
    m_pHide->create();
    m_pHide->setFont(hFont);

    m_p3DView = new COpenGl(m_hInstance, m_hParent, 0, 0, 10, 10, ID_3DVIEW);
    m_p3DView->create();

    hFont = CreateFont(12, 5, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, NULL, "Fixed days");
    m_pLblController = new CLabel(m_hInstance, m_hParent, 0, 0, 10, 10, ID_LBLC);
    m_pLblController->create();
    m_pLblController->setText("Controller value");
    m_pLblController->setFont(hFont);

    m_pController = new CSlider(m_hInstance, m_hParent, 0, 0, 10, 10, ID_CONTROLLER);
    m_pController->create(NULL, TBS_ENABLESELRANGE);
    m_pController->init(0, 100);
    m_pController->setSel(0);

    m_pLblAnimation = new CLabel(m_hInstance, m_hParent, 0, 0, 10, 10, ID_LBLC);
    m_pLblAnimation->create();
    m_pLblAnimation->setText("Animation speed");
    m_pLblAnimation->setFont(hFont);

    m_pStartStop = new CButton(m_hInstance, m_hParent, 0, 0, 10, 10, ID_STARTSTOP);
    m_pStartStop->create();
    if (settings.animate)
        m_pStartStop->setText("Stop");
    else
        m_pStartStop->setText("Start");
    m_pStartStop->setFont(m_pPanel->getFont());

    m_pSpeed = new CSlider(m_hInstance, m_hParent, 0, 0, 10, 10, ID_SPEED);
    m_pSpeed->create(NULL, TBS_ENABLESELRANGE);
    m_pSpeed->init(0, 100);
    m_pSpeed->setSel(10);

    m_pFrames = new CSlider(m_hInstance, m_hParent, 0, 0, 10, 10, ID_FRAMES);
    m_pFrames->create(NULL, TBS_AUTOTICKS | TBS_ENABLESELRANGE);
    m_pFrames->init(1, 100);

    m_pStatus = new CStatusBar(m_hInstance, m_hParent, ID_STATUS);
    m_pStatus->create();
    m_pStatus->addOneItem("GUI initialized...");
}

CCManager::~CCManager()
{
}

/////////////////////////////////////
// Memberfuncties                  //
/////////////////////////////////////

void CCManager::drawOpenGL()
{
    switch (m_nView)
    {
        case VIEW_MODEL:
            m_p3DView->draw();
            m_pFrames->setSel(m_pModel->GetFrame());
            break;

        case VIEW_MATERIAL:
            m_p3DView->drawmaterial();
            m_pStatus->setItemText("", 2);
            break;
    }
}

void CCManager::resize(
    WPARAM wParam,
    LPARAM lParam)
{
    m_nWidth = LOWORD(lParam);
    m_nHeight = HIWORD(lParam);

    if (settings.hidepanel)
    {
        m_pPanel->resize(0, 15, 198, m_nHeight - 165);
        m_p3DView->resize(200, 0, m_nWidth - 200, m_nHeight - 62);

        m_pHide->resize(185, 0, 12, 12);
        m_pHide->setText("<");
    }
    else
    {
        m_pPanel->resize(0, 15, 0, m_nHeight - 165);
        m_p3DView->resize(15, 0, m_nWidth - 15, m_nHeight - 62);

        m_pHide->resize(0, 0, 10, 10);
        m_pHide->setText(">");
    }
    m_pStatus->resize(wParam, lParam);
    m_pLblController->resize(25, m_nHeight - 145, 160, 10);
    m_pController->resize(20, m_nHeight - 130, 160, 20);
    m_pLblAnimation->resize(25, m_nHeight - 95, 100, 10);
    m_pStartStop->resize(135, m_nHeight - 105, 40, 20);
    m_pSpeed->resize(20, m_nHeight - 80, 160, 20);
    m_pFrames->resize(0, m_nHeight - 60, m_nWidth, 40);

    m_p3DView->setFocus();
    m_pLblController->hide(!settings.hidepanel);
    m_pController->hide(!settings.hidepanel);
    m_pLblAnimation->hide(!settings.hidepanel);
    m_pStartStop->hide(!settings.hidepanel);
    m_pSpeed->hide(!settings.hidepanel);
}

void CCManager::togglePanel()
{
    settings.hidepanel = !settings.hidepanel;
    if (settings.hidepanel)
    {
        m_pPanel->resize(0, 15, 198, m_nHeight - 165);
        m_p3DView->resize(200, 0, m_nWidth - 200, m_nHeight - 62);

        m_pHide->resize(185, 0, 12, 12);
        m_pHide->setText("<");
    }
    else
    {
        m_pPanel->resize(0, 15, 0, m_nHeight - 165);
        m_p3DView->resize(15, 0, m_nWidth - 15, m_nHeight - 62);

        m_pHide->resize(0, 0, 12, 12);
        m_pHide->setText(">");
    }
    m_p3DView->setFocus();
    m_pLblController->hide(!settings.hidepanel);
    m_pController->hide(!settings.hidepanel);
    m_pLblAnimation->hide(!settings.hidepanel);
    m_pStartStop->hide(!settings.hidepanel);
    m_pSpeed->hide(!settings.hidepanel);
}

bool CCManager::toggleAnimatie()
{
    settings.animate = !settings.animate;
    if (settings.animate)
    {
        m_pStartStop->setText("Stop");
    }
    else
    {
        m_pStartStop->setText("Start");
    }
    return settings.animate;
}

bool CCManager::initGui(
    StudioModel *pModel)
{
    this->m_pModel = pModel;

    if (!m_p3DView->init(m_pModel))
    {
        return false;
    }

    char str[64];
    HTREEITEM hItem;

    // REFERENCE
    studiohdr_t *hdr = m_pModel->getStudioHeader();

    sprintf_s(str, 64, "References (%i)", hdr->numbodyparts);
    m_hReference = m_pPanel->addItem(str, TVGN_ROOT, 0);
    mstudiobodyparts_t *pbodyparts = (mstudiobodyparts_t *)((byte *)hdr + hdr->bodypartindex);
    if (hdr->numbodyparts > 0)
    {
        for (int i = 0; i < hdr->numbodyparts; i++)
        {
            hItem = m_pPanel->addItem(pbodyparts[i].name, m_hReference, 0);
            for (int j = 0; j < pbodyparts[i].nummodels; j++)
            {
                sprintf_s(str, 64, "Submodel %d", j);
                m_pPanel->addItem(str, hItem, 0);
            }
        }
    }
    // END REFERENCE

    // SKIN
    if (hdr)
    {
        sprintf_s(str, 64, "Skins (%i)", hdr->numskinfamilies);
        m_hSkins = m_pPanel->addItem(str, TVGN_ROOT, 0);
        for (int i = 0; i < hdr->numskinfamilies; i++)
        {
            sprintf_s(str, 64, "Skin %d", i + 1);
            m_pPanel->addItem(str, m_hSkins, 0);
        }
    }
    // END SKIN

    // CONTROLLER
    if (hdr)
    {
        sprintf_s(str, 64, "Controllers (%i)", hdr->numbonecontrollers);
        m_hControllers = m_pPanel->addItem(str, TVGN_ROOT, 0);
        mstudiobonecontroller_t *pbonecontrollers = (mstudiobonecontroller_t *)((byte *)hdr + hdr->bonecontrollerindex);
        for (int i = 0; i < hdr->numbonecontrollers; i++)
        {
            if (pbonecontrollers[i].index == 4)
                sprintf_s(str, 64, "Mouth %d", i);
            else
                sprintf_s(str, 64, "Controller %d", i);
            m_pPanel->addItem(str, m_hControllers, 0);
        }
    }
    // END CONTROLLER

    // SEQUENCE
    if (hdr)
    {
        sprintf_s(str, 64, "Sequences (%i)", hdr->numseq);
        m_hSequence = m_pPanel->addItem(str, TVGN_ROOT, 0);
        mstudioseqdesc_t *pseqdescs = (mstudioseqdesc_t *)((byte *)hdr + hdr->seqindex);
        for (int i = 0; i < hdr->numseq; i++)
        {
            HTREEITEM hSequence = m_pPanel->addItem(pseqdescs[i].label, m_hSequence, 0);
        }
        m_pFrames->init(0, pseqdescs[0].numframes - 1);
    }
    // END SEQUENCE

    // MATERIAL
    hdr = m_pModel->getTextureHeader();
    if (hdr)
    {
        sprintf_s(str, 64, "Materials (%i)", hdr->numskinref);
        m_hMaterial = m_pPanel->addItem(str, TVGN_ROOT, 0);
        mstudiotexture_t *ptextures = (mstudiotexture_t *)((byte *)hdr + hdr->textureindex);
        for (int i = 0; i < hdr->numtextures; i++)
        {
            m_pPanel->addItem(ptextures[i].name, m_hMaterial, 0);
        }
    }
    // END MATERIAL

    // BONES
    hdr = m_pModel->getStudioHeader();
    if (hdr)
    {
        sprintf_s(str, 64, "Bones (%i)", hdr->numbones);
        m_hBones = m_pPanel->addItem(str, TVGN_ROOT, 0);
        mstudiobone_t *pbones = (mstudiobone_t *)((byte *)hdr + hdr->boneindex);
        for (int i = 0; i < hdr->numbones; i++)
        {
            m_pPanel->addItem(pbones[i].name, m_hBones, 0);
        }
    }
    // END BONES

    // HITBOXES
    hdr = m_pModel->getStudioHeader();
    if (hdr)
    {
        sprintf_s(str, 64, "Hitboxes (%i)", hdr->numhitboxes);
        m_hHitboxes = m_pPanel->addItem(str, TVGN_ROOT, 0);
        for (int i = 0; i < hdr->numhitboxes; i++)
        {
            sprintf_s(str, 64, "Hitbox %i", i);
            m_pPanel->addItem(str, m_hHitboxes, 0);
        }
    }

    // END HITBOXES
    return true;
}

void CCManager::clearGui()
{
    char buffer[255];

    m_p3DView->clear();
    m_pPanel->reset();

    m_pFrames->init(1, 100);
    m_pFrames->setSel(0);

    m_pSpeed->init(0, 100);
    m_pSpeed->setSel(int(settings.speed * 10));
    sprintf_s(buffer, 255, "Animation speed : %i", m_pSpeed->getSel());
    m_pLblAnimation->setText(buffer);

    m_pController->init(0, 10);
    m_pController->setSel(0);

    m_pLblController->setText("Controller value : 0");
}

void CCManager::ItemSelected(
    WPARAM wParam,
    LPARAM lParam)
{
    char buffer[255];

    string seltext;
    string partext;

    seltext = m_pPanel->getItemText(m_pPanel->getSelection());
    partext = m_pPanel->getItemText(m_pPanel->getParent(m_pPanel->getSelection()));

    // REFERENCE
    if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hReference)
    {
        studiohdr_t *hdr = m_pModel->getStudioHeader();
        if (hdr)
        {
            mstudiobodyparts_t *pbodyparts = (mstudiobodyparts_t *)((byte *)hdr + hdr->bodypartindex);
            for (int i = 0; i < hdr->numbodyparts; i++)
            {
                if (strcmp(pbodyparts[i].name, seltext.c_str()) == 0)
                {
                    m_pModel->SetBodygroup(i, 0);
                }
            }
        }
    }

    // REFERENCE SUBMODEL
    else if (m_pPanel->getParent(m_pPanel->getParent(m_pPanel->getSelection())) == m_hReference)
    {
        studiohdr_t *hdr = m_pModel->getStudioHeader();
        if (hdr)
        {
            mstudiobodyparts_t *pbodyparts = (mstudiobodyparts_t *)((byte *)hdr + hdr->bodypartindex);
            for (int i = 0; i < hdr->numbodyparts; i++)
            {
                if (strcmp(pbodyparts[i].name, partext.c_str()) == 0)
                {
                    m_pModel->SetBodygroup(i, getIndex(seltext));
                }
            }
        }
    }

    // SKINS
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hSkins)
    {
        m_pModel->SetSkin(getIndex(seltext) - 1);
    }

    // CONTROLLER
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hControllers)
    {
        studiohdr_t *hdr = m_pModel->getStudioHeader();
        if (hdr)
        {
            mstudiobonecontroller_t *pbonecontrollers = (mstudiobonecontroller_t *)((byte *)hdr + hdr->bonecontrollerindex);

            if (strstr(seltext.c_str(), "Mouth") != NULL)
            {
                m_nController = 4;
                m_pModel->SetMouth(0.0f);
            }
            else
            {
                m_nController = getIndex(seltext);
                m_pModel->SetController(getIndex(seltext), 0.0f);
            }
            m_pController->init((int)pbonecontrollers[getIndex(seltext)].start, (int)pbonecontrollers[getIndex(seltext)].end);
            sprintf(buffer, "Controller value : %i", m_pController->getSel());
            m_pLblController->setText(buffer);
        }
    }

    // SEQUENCE
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hSequence)
    {
        studiohdr_t *hdr = m_pModel->getStudioHeader();
        if (hdr)
        {
            mstudioseqdesc_t *pseqdescs = (mstudioseqdesc_t *)((byte *)hdr + hdr->seqindex);
            for (int i = 0; i < hdr->numseq; i++)
            {
                if (strcmp(pseqdescs[i].label, seltext.c_str()) == 0)
                {
                    m_pModel->SetSequence(i);
                    m_pFrames->init(0, pseqdescs[i].numframes - 1);
                }
            }
        }
    }

    // BONES
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hBones)
    {
        studiohdr_t *hdr = m_pModel->getStudioHeader();
        if (hdr)
        {
            mstudiobone_t *pbones = (mstudiobone_t *)((byte *)hdr + hdr->boneindex);
            for (int i = 0; i < hdr->numbones; i++)
            {
                if (strcmp(pbones[i].name, seltext.c_str()) == 0)
                {
                    m_pModel->SetSelBone(i);
                }
            }
        }
    }

    // MATERIALS
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hMaterial)
    {
        studiohdr_t *hdr = m_pModel->getTextureHeader();
        if (hdr)
        {
            mstudiotexture_t *ptextures = (mstudiotexture_t *)((byte *)hdr + hdr->textureindex);
            for (int i = 0; i < hdr->numtextures; i++)
            {
                if (strcmp(ptextures[i].name, seltext.c_str()) == 0)
                {
                    m_p3DView->setMaterial(i);
                    m_nView = VIEW_MATERIAL;
                    return;
                }
            }
        }
    }

    // DESELBONES
    else if (strcmp(seltext.c_str(), "Bones") == 0)
    {
        m_pModel->SetSelBone(-1);
    }

    // HITBOXES
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hHitboxes)
    {
        m_pModel->SetSelHitbox(getIndex(seltext));
    }

    // DESELHITBOXES
    else if (strcmp(seltext.c_str(), "Hitboxes") == 0)
    {
        m_pModel->SetSelHitbox(-1);
    }
    m_nView = VIEW_MODEL;
}

int CCManager::getIndex(string str)
{
    char *result;
    char *temp;

    temp = const_cast<char *>(str.c_str());

    for (size_t i = strlen(temp) - 1; i > 0; i--)
    {
        if (!(temp[i] < 58 && temp[i] > 47))
        {
            result = new char[strlen(temp) - i];
            size_t j = 0;
            for (; j < strlen(temp) - i; j++)
            {
                result[j] = temp[j + i + 1];
            }
            result[j] += '\0';
            return atoi(result);
        }
    }
    return 0;
}

void CCManager::SliderScoll(
    HWND hWnd,
    WPARAM wParam,
    LPARAM lParam)
{
    char buffer[255];
    if ((HWND)lParam == m_pFrames->getHandle())
    {
        m_pModel->SetFrame(m_pFrames->getSel());
    }
    else if ((HWND)lParam == m_pSpeed->getHandle())
    {
        settings.speed = (float)m_pSpeed->getSel() / 10;
        sprintf_s(buffer, 255, "Animation speed : %i", m_pSpeed->getSel());
        m_pLblAnimation->setText(buffer);
    }
    else if ((HWND)lParam == m_pController->getHandle())
    {
        studiohdr_t *hdr = m_pModel->getStudioHeader();
        if (hdr)
        {
            if (m_nController == 4)
            {
                m_pModel->SetMouth(m_pController->getSel());
            }
            else
            {
                m_pModel->SetController(m_nController, m_pController->getSel());
            }

            sprintf_s(buffer, 255, "Controller value : %i", m_pController->getSel());
            m_pLblController->setText(buffer);
        }
    }
}

int CCManager::getTreeSelectedType()
{
    // REFERENCE
    if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hReference)
    {
        return TREETYPE_REF;
    }

    // REFERENCE SUBMODEL
    else if (m_pPanel->getParent(m_pPanel->getParent(m_pPanel->getSelection())) == m_hReference)
    {
        return TREETYPE_REF_SUB;
    }

    // SKINS
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hSkins)
    {
        return TREETYPE_SKIN;
    }

    // CONTROLLER
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hControllers)
    {
        return TREETYPE_CONT;
    }

    // SEQUENCE
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hSequence)
    {
        return TREETYPE_SEQ;
    }

    // BONES
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hBones)
    {
        return TREETYPE_BONE;
    }

    // MATERIALS
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hMaterial)
    {
        return TREETYPE_MAT;
    }

    // HITBOXES
    else if (m_pPanel->getParent(m_pPanel->getSelection()) == m_hHitboxes)
    {
        return TREETYPE_HIT;
    }
    else
    {
        return -1;
    }
}

string CCManager::getTreeSelectedText()
{
    return m_pPanel->getItemText(m_pPanel->getSelection());
}

string CCManager::getTreeParentText()
{
    return m_pPanel->getItemText(m_pPanel->getParent(m_pPanel->getSelection()));
}

void CCManager::setView(
    int view)
{
    this->m_nView = view;
}

void CCManager::setStatus(
    const char *text,
    ...)
{
    va_list argp;
    char result[256];

    va_start(argp, text);
    vsprintf_s(result, 256, text, argp);
    va_end(argp);

    m_pStatus->setItemText(result, 1);
}

void CCManager::centerview()
{
    m_p3DView->centerview();
}

void CCManager::vmodelview()
{
    m_p3DView->vmodelview();
}

void CCManager::statusPan()
{
    char str[255];

    sprintf_s(str, 255, "PAN : %d", m_p3DView->getPan());

    m_pStatus->setItemText(str, 1);
}
