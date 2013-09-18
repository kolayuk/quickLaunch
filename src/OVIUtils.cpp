/*
 ============================================================================
 Name		: OVIUtils.cpp
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : COVIUtils implementation
 ============================================================================
 */

#include "OVIUtils.h"
#include <quickLauncher.rsg>
#include <aknsutils.h>
#include <aknutils.h>
#include <aknsdrawutils.h>
#include <apgcli.h>
#include <Main.hrh>
#include <apgtask.h>
#include <const.h>
#include <aknquerydialog.h>
#include <quickLauncher_aif.mbg>

CAboutContainer::CAboutContainer()
{
}

void CAboutContainer::ConstructL(const TRect& aRect)
{
	CreateWindowL();
	iBgContext = CAknsBasicBackgroundControlContext::NewL(KAknsIIDQsnBgAreaMain, aRect, EFalse);
	TFileName fileName = KImageFileName();

#ifndef __WINS__
	Utils::CompleteWithAppPathM(fileName);
#endif

	AknIconUtils::CreateIconL(iBitmap, iMask, fileName, 16384, 16385);
	SetIconSize(aRect);
	SetRect(aRect);
	ActivateL();
}

CAboutContainer::~CAboutContainer()
{
	delete iBgContext;
	delete iBitmap;
	delete iMask;
}
void CAboutContainer::ConstructFromResourceL(TResourceReader& aReader)
	{
	CreateWindowL();
	TRect r;
	AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,r);
	iBgContext = CAknsBasicBackgroundControlContext::NewL(KAknsIIDQsnBgAreaMain, r, EFalse);
	TFileName fileName = KImageFileName();
	
	#ifndef __WINS__
	Utils::CompleteWithAppPathM(fileName);
	#endif

	AknIconUtils::CreateIconL(iBitmap, iMask, fileName, EMbmQuicklauncher_aifLogo, EMbmQuicklauncher_aifLogo_mask);
	SetIconSize(r);
	SetRect(r);
	ActivateL();
	}
TTypeUid::Ptr CAboutContainer::MopSupplyObject(TTypeUid aId)
{
	if (iBgContext != NULL)
	{
		return MAknsControlContext::SupplyMopObject(aId, iBgContext);
	}
	return CCoeControl::MopSupplyObject(aId);
}

void CAboutContainer::DrawStringCentered(const CFont *font, const TDesC &string, TInt& top, TInt width, CWindowGc& gc) const
{
	CArrayFixFlat<TPtrC> *arrStr = new CArrayFixFlat<TPtrC> (5);
	CleanupStack::PushL(arrStr);
	TRAPD(err, AknTextUtils::WrapToArrayL(string, width - 10, *font, *arrStr));
	if (err == KErrNone)
	{
		for (int i = 0; i < arrStr->Count(); i++)
		{
			TInt txtWidth = font->TextWidthInPixels((*arrStr)[i]);
			TPoint pt(width / 2 - txtWidth / 2, top);
			gc.DrawText((*arrStr)[i], pt);
			top += font->HeightInPixels() + 5;
		}
	}
	CleanupStack::PopAndDestroy(arrStr);
}

void CAboutContainer::SizeChanged()
{
	if (iBgContext != NULL)
	{
		iBgContext->SetRect(Rect());
	}
	TRect r;
	AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,r);
	if (Position().iY!=r.iTl.iY){SetPosition(r.iTl);}
	CCoeControl::SizeChanged();
}

void CAboutContainer::HandleResourceChange(TInt aType)
{
	if (aType == KEikDynamicLayoutVariantSwitch)
	{
		TRect rect;
		AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
		SetRect(rect);
		SetIconSize(rect);
	}
}

void CAboutContainer::SetIconSize(const TRect& aRect)
{
	TInt icon_width = (TInt)(aRect.Width() / 2);
	TInt icon_height = (TInt)(icon_width / 5.35);
	AknIconUtils::SetSize(iBitmap, TSize(icon_width, icon_height), EAspectRatioPreserved);
	AknIconUtils::SetSize(iMask, TSize(icon_width, icon_height), EAspectRatioPreserved);
}

TInt CAboutContainer::CountComponentControls() const
{
	return NULL; // return nbr of controls inside this container
}

CCoeControl* CAboutContainer::ComponentControl(TInt aIndex) const
{
	switch (aIndex)
	{
		default:
			return NULL;
	}
}

void CAboutContainer::Draw(const TRect& aRect) const
{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Redraw the background using the default skin
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
	AknsDrawUtils::Background(skin, cc, this, gc, aRect);

	TRgb textColor;
	AknsUtils::GetCachedColor(skin, textColor, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6); // text #6	main area	main area texts	#215

	TRect rect(iBitmap->SizeInPixels());
	TInt client_width = aRect.Width();
	TInt bitmap_width = rect.Width();
	TInt bitmap_height = rect.Height();
	TInt x_coord = (client_width - bitmap_width) / 2;
	TInt y_coord = 10;

	TRect canvas = TRect(x_coord - 5, y_coord - 5, x_coord + bitmap_width + 5, y_coord + bitmap_height + 5);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.SetBrushColor(KRgbWhite);
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	gc.SetPenColor(KRgbBlack);
	gc.DrawRect(canvas);

	gc.BitBltMasked(TPoint(x_coord, y_coord), iBitmap, rect, iMask, 0);

	gc.SetPenColor(textColor);

	const CFont *font;
	TFileName buff;
	TInt width = Rect().Width();
	font = CEikonEnv::Static()->TitleFont();
	gc.UseFont(font);
	TFileName buff_format;
	CEikonEnv::Static()->ReadResource(buff_format, R_PROGRAM_NAME);

	buff.Format(buff_format, VER_MAJOR, VER_MINOR);

	TInt top = font->HeightInPixels() + bitmap_height + 25;
	DrawStringCentered(font, buff, top, width, gc);
	gc.DiscardFont();

	font = CEikonEnv::Static()->DenseFont();
	gc.UseFont(font);

	CEikonEnv::Static()->ReadResource(buff, R_PUBLISHER_INFORMATION);
	DrawStringCentered(font, buff, top, width, gc);

	CEikonEnv::Static()->ReadResource(buff, R_AUTOR_INFORMATION);
	DrawStringCentered(font, buff, top, width, gc);

	CEikonEnv::Static()->ReadResource(buff, R_COPYRIGHT_INFORMATION);
	DrawStringCentered(font, buff, top, width, gc);

	CEikonEnv::Static()->ReadResource(buff, R_SUPPORT_INFORMATION);
	DrawStringCentered(font, buff, top, width, gc);

	CEikonEnv::Static()->ReadResource(buff, R_EMAIL_INFORMATION);
	DrawStringCentered(font, buff, top, width, gc);
	gc.DiscardFont();
}


namespace Utils
{
	void CompleteWithAppPathM(TDes &aPath)
	{
		TFileName fullAppName = RProcess().FileName();
		TParse parse;
		TParse parse1;
		parse.Set(fullAppName, NULL, NULL);
		parse1.Set(aPath, NULL, NULL);

		if(parse1.DrivePresent())
		{
			aPath[0] = parse.Drive()[0];
		}
		else
		{
			aPath.Insert(0, parse.Drive());
		}
#ifdef __WINS__
		aPath[0] = 'c';
#endif
	}

#ifndef __WINS__
	const TInt KBrowserUid = 0x10008D39;
#else
	const TInt KBrowserUid = 0x1020724D;
#endif

	void OpenBrowser(const TDesC& aUrl)
	{
		TUid id(TUid::Uid(KBrowserUid));
		TApaTaskList taskList(CEikonEnv::Static()->WsSession());
		TApaTask task = taskList.FindApp(id);

		if (task.Exists())
		{
			HBufC8* param8 = HBufC8::NewLC(100);
			param8->Des().Append(aUrl);
			// Uid is not used
			task.SendMessage(TUid::Uid(0), *param8);
			CleanupStack::PopAndDestroy();
		}
		else
		{
			RApaLsSession appArcSession;
			// connect to AppArc server
			User::LeaveIfError(appArcSession.Connect());
			TThreadId id;
			TInt ret = appArcSession.StartDocument(aUrl, TUid::Uid(KBrowserUid), id);
			appArcSession.Close();
		}
	}
}



CAboutDialog::CAboutDialog(): CAknDialog()
	{
	// No implementation required
	}

TInt CAboutDialog::RunDlgLD()
    {
	CAboutDialog* dlg = new (ELeave) CAboutDialog();
	return dlg->ExecuteLD(R_ABOUT_DIALOG);
	
    }
 
void CAboutDialog::PostLayoutDynInitL()
	{
	CAboutContainer* control = (CAboutContainer*)Control(KMyCustomCtlId);
	TRect r;
	AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,r);
	control->HandleResourceChange(KEikDynamicLayoutVariantSwitch);
	control->MakeVisible(ETrue);
	}

SEikControlInfo CAboutDialog::CreateCustomControlL(TInt aControlType)
    {
    SEikControlInfo controlInfo;
    controlInfo.iControl = NULL ;
    controlInfo.iTrailerTextId = 0 ;
    controlInfo.iFlags = 0 ;
 
    switch (aControlType)
        {
        // CMyControl custom control type (defined in multiviews.hrh)
        case KMyCustomCtl:
            {
            controlInfo.iControl = new (ELeave)CAboutContainer();
            break;
            }
        default:
            break;
        }
    return controlInfo;
    }
TBool CAboutDialog::OkToExitL(TInt aButtonID)
	{
	return ETrue; //breakpoint - ok
	}


void OVIUtils::GoToOvi()
	{

	TFileName message;
	CEikonEnv::Static()->ReadResource(message, R_BUF_GOTOBUY_DLGMSG);
	CAknQueryDialog* dialog = new (ELeave) CAknQueryDialog();
	dialog->SetPromptL(message);
	TInt ret = dialog->ExecuteLD(R_CONTINUE_CANCEL_QUERY);
	if (ret == EEikBidOk)
		{
		Utils::OpenBrowser(KOurPageInOvi);
		}
	}
void OVIUtils::AboutDialog()
	{
	CAboutDialog::RunDlgLD();
	}
