/*
 ============================================================================
 Name		: AppContainer.cpp
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : CAppContainer implementation
 ============================================================================
 */

#include "AppContainer.h"
#include <aknbutton.h>
#include <apgcli.h>
#include <gulicon.h>
#include <aknsutils.h>
#include <akniconutils.h>
#include <const.h>
#include <avkon.mbg>
#include <aknutils.h>
#include <aknsdrawutils.h>
#include <quickLauncherAppUi.h>
#include <quickLauncher_aif.mbg>
#include <logger.h>
#include <touchfeedback.h>

CAppContainer::CAppContainer():CCoeControl()
	{
	// No implementation required
	}

CAppContainer::~CAppContainer()
	{
	delete iBgContext;
	delete iHider;
	delete iIcon;
	}

CAppContainer* CAppContainer::NewLC(RWindowGroup* aWg)
	{
	CAppContainer* self = new (ELeave) CAppContainer();
	CleanupStack::PushL(self);
	self->ConstructL(aWg);
	return self;
	}

CAppContainer* CAppContainer::NewL(RWindowGroup* aWg)
	{
	CAppContainer* self = CAppContainer::NewLC(aWg);
	CleanupStack::Pop(); // self;
	return self;
	}

void CAppContainer::ConstructL(RWindowGroup* aWg)
	{
	_LOG(_L("CAppContainer::ConstructL"));
	iWinGroup=aWg;
	CreateWindowL(aWg);
	InitComponentArrayL();
	Window().SetTransparencyAlphaChannel();
	TRgb col=KRgbWhite;col.SetAlpha(0);
	Window().SetBackgroundColor(col);
	MakeVisible(EFalse);
	SetExtentToWholeScreen();
	iBgContext = CAknsBasicBackgroundControlContext::NewL( KAknsIIDQsnFrPopupPreview,Rect(),ETrue);
	iHider=CContainerHider::NewL(aWg);
	Window().SetOrdinalPosition(0);
	LoadIcon();
	iDrawSelect=-1;
	ActivateL();
	}
void CAppContainer::AddApp(TUid aUid)
	{
	_LOGDATA(_L("CAppContainer::AddApp(uid=%d)"),aUid.iUid);
	CreateButton(aUid);
	}
void CAppContainer::CreateButton(TUid aUid)
	{
	CContainerButton* Button=CContainerButton::NewL(this);
	Button->SetSize(KButtonSize);
	TInt direction=((CquickLauncherAppUi*)CEikonEnv::Static()->AppUi())->iSettings[2];
	if (direction==0||direction==1)
		{Button->SetPosition(TPoint(0,(Components().Count()*(KButtonSize.iHeight+KBetween))));}
	else if (direction==2||direction==3)
		{Button->SetPosition(TPoint((Components().Count()*(KButtonSize.iWidth+KBetween)),0));}
	Button->SetObserver(this);
	Button->MakeVisible(ETrue);
	
	CGulIcon* icon;
	TRAPD(erreasy,icon=LoadAppIconEasy(aUid));
	if (erreasy==KErrNotFound)
		{
		CFbsBitmap*	AppIcon(NULL);
		CFbsBitmap*	AppIconMsk(NULL);
		AknIconUtils::CreateIconL(AppIcon,AppIconMsk,AknIconUtils::AvkonIconFileName(), EMbmAvkonQgn_indi_no_signal, EMbmAvkonQgn_indi_no_signal_mask);
		AknIconUtils::SetSize(AppIcon,KButtonSize);
		AknIconUtils::SetSize(AppIconMsk,KButtonSize);
		icon=CGulIcon::NewL(AppIcon,AppIconMsk);
		}
	else if (erreasy!=KErrNone){TRAPD(errhard,icon=LoadAppIconHard(aUid));}
	Button->SetIcon(icon);
	Components().AppendLC(Button,aUid.iUid);
	CleanupStack::Pop(Button);
	
	}


CGulIcon* CAppContainer::LoadAppIconEasy(TUid aUid)
	{
	_LOG(_L("LoadAppIconEasy"));
	RApaLsSession ls;
	ls.Connect();
	TApaAppInfo info;
	User::LeaveIfError(ls.GetAppInfo(info,aUid));
	HBufC* path=HBufC::NewL(255);
	TInt err=ls.GetAppIcon(aUid,path);
	_LOGDATA(_L("Err: %d"),err);
	_LOGDATA(_L("Icon path: %S"),&path->Des());
	User::LeaveIfError(err);
	CFbsBitmap*	AppIcon(NULL);
	CFbsBitmap*	AppIconMsk(NULL);
	//MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	if ((err==KErrNone)&&(path->Length()!=0))
		{AknsUtils::CreateAppIconLC(AknsUtils::SkinInstance(),aUid, EAknsAppIconTypeContext,AppIcon,AppIconMsk);}
	else {User::Leave(KErrNotFound);}
	AknIconUtils::SetSize(AppIcon,KButtonSize);
	AknIconUtils::SetSize(AppIconMsk,KButtonSize);		
	CleanupStack::Pop(2);
	CGulIcon* icon=CGulIcon::NewL(AppIcon,AppIconMsk);
	ls.Close();
	return icon;
	}
CGulIcon* CAppContainer::LoadAppIconHard(TUid aUid)
	{
	_LOG(_L("LoadAppIconHard"));
	RApaLsSession ls;
	ls.Connect();
	CGulIcon *retval = NULL;
	CArrayFixFlat<TSize> *array = new CArrayFixFlat<TSize>(3);
	CleanupStack::PushL(array);
	TInt err = ls.GetAppIconSizes(aUid, *array);
	_LOGDATA(_L("GetAppIconSizes err = %d"), err);
	_LOGDATA(_L("%d icons found"),array->Count());
	if(err == KErrNone && array->Count() > 0)
		{
		CApaMaskedBitmap *bitmap = CApaMaskedBitmap::NewLC();
		err = ls.GetAppIcon(aUid, (*array)[0], *bitmap);
		_LOGDATA(_L("GetAppIcon err = %d"), err);
		if(err == KErrNone)
			{
			CFbsBitmap* bmp = new (ELeave) CFbsBitmap();
			CleanupStack::PushL(bmp);
			CFbsBitmap* bmp_mask = new (ELeave) CFbsBitmap();
			CleanupStack::PushL(bmp_mask);
			User::LeaveIfError(bmp->Create(bitmap->SizeInPixels(), bitmap->DisplayMode()));
			User::LeaveIfError(bmp_mask->Create(bitmap->Mask()->SizeInPixels(), bitmap->Mask()->DisplayMode()));
			CopyBitmapL(bitmap, bmp);
			CopyBitmapL(bitmap->Mask(), bmp_mask);
			retval = CGulIcon::NewL(bmp, bmp_mask);
			CleanupStack::Pop(2); // bmp, bmp_mask
			}
			CleanupStack::PopAndDestroy(bitmap);
		}
		CleanupStack::PopAndDestroy(array);
		ls.Close();
		return retval;
	}

void CAppContainer::Draw(const TRect& aRect) const
	{
	_LOGDATA(_L("CAppContainer::Draw %d"),iDrawSelect);
	CWindowGc& gc=SystemGc();
	gc.Clear();
	// Redraw the background using the default skin
	//MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	// MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
	 //AknsDrawUtils::Background( skin, cc, this, gc, aRect );
	AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(),gc,Rect(),Rect(),KAknsIIDQsnFrPopupSub,KAknsIIDDefault);
	if (iDrawSelect!=-1)
		{
		TRect r=ComponentControl(iDrawSelect)->Rect();
		_LOGDATA2(_L("Selection x,y: %d %d"),r.iTl.iX,r.iTl.iY);
		_LOGDATA2(_L("Selection w,h: %d %d"),r.Size().iWidth,r.Size().iHeight);
		AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(),gc,r,r,KAknsIIDQsnFrButtonTb,KAknsIIDDefault);
		}
	//gc.BitBltMasked(aRect.iTl,iIcon->Bitmap(),TRect(aRect.iTl,iIcon->Bitmap()->SizeInPixels()),iIcon->Mask(),EFalse);
	}
TTypeUid::Ptr CAppContainer::MopSupplyObject(TTypeUid aId)
	{
	if (iBgContext )
	{
		return MAknsControlContext::SupplyMopObject( aId, iBgContext );
	}
	return CCoeControl::MopSupplyObject(aId);
}
void CAppContainer::SizeChanged()
{
	if(iBgContext)
	{
	iBgContext->SetRect(Rect());
	if ( &Window() )
		{
		iBgContext->SetParentPos( PositionRelativeToScreen() );
		}
	}
	if (iIcon)
		{
		AknIconUtils::SetSize(iIcon->Bitmap(),Size(),EAspectRatioNotPreserved);
		AknIconUtils::SetSize(iIcon->Mask(),Size(),EAspectRatioNotPreserved);
		}
}
void CAppContainer::Clear()
	{
	Components().ResetAndDestroy();
	}

void CAppContainer::HandleControlEventL(CCoeControl* aControl, enum TCoeEvent aEvent)
	{
	if (aEvent==EEventStateChanged)
		{
		((CquickLauncherAppUi*)CEikonEnv::Static()->AppUi())->ShowContainer(EFalse,EFalse);
		MTouchFeedback::Instance()->InstantFeedback(ETouchFeedbackBasic);
		Launch(TUid::Uid(Components().Id(*aControl)));
		}
	}
void CAppContainer::Launch(TUid aUid)
	{
	_LOGDATA(_L("CAppContainer::Launch(%d)"),aUid);
    RApaLsSession apaLsSession;
    User::LeaveIfError(apaLsSession.Connect());
    CleanupClosePushL(apaLsSession);
    TApaAppInfo appInfo;
    TInt retVal = apaLsSession.GetAppInfo(appInfo, aUid);
 
    if(retVal == KErrNone)
        {
        CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
        cmdLine->SetExecutableNameL(appInfo.iFullName);
        _LOGDATA(_L("App path: %S"),&appInfo.iFullName);
        cmdLine->SetCommandL(EApaCommandRun);
        TInt err=apaLsSession.StartApp(*cmdLine);
        _LOGDATA(_L("Launch err: %d"),err);
        CleanupStack::PopAndDestroy(cmdLine);
        }
    else
        {
        // The application not found!
        }
    CleanupStack::PopAndDestroy(&apaLsSession);
	}
void CAppContainer::HandlePointerEventL(const TPointerEvent& aEvent)
	{
	for (TInt i=0;i<Components().Count();i++)
		{
		if (ComponentControl(i)->Rect().Contains(aEvent.iPosition))
			{
			if (aEvent.iType==aEvent.EButton1Down)
				{
				iDrawSelect=i;
				DrawNow();
				iDrawSelect=-1;
				}
			else if (aEvent.iType==aEvent.EButton1Up)
				{
				((CquickLauncherAppUi*)CEikonEnv::Static()->AppUi())->ShowContainer(EFalse,EFalse);
				MTouchFeedback::Instance()->InstantFeedback(ETouchFeedbackBasic);
				Launch(TUid::Uid(Components().At(i).iId));
				}
			}
		}
	}
void CAppContainer::LoadIcon()
	{
	CFbsBitmap* mask;
	CFbsBitmap* icon;
	_LIT(KPath,"\\resource\\apps\\quickLauncher_aif.mif");
	AknIconUtils::CreateIconL(icon,mask,KPath,EMbmQuicklauncher_aifBg,EMbmQuicklauncher_aifBg_mask);
	AknIconUtils::SetSize(icon,Size(),EAspectRatioNotPreserved);
	AknIconUtils::SetSize(mask,Size(),EAspectRatioNotPreserved);
	iIcon=CGulIcon::NewL(icon,mask);
	}

CContainerHider::CContainerHider():CCoeControl()
	{
	// No implementation required
	}

CContainerHider::~CContainerHider()
	{
	}

CContainerHider* CContainerHider::NewLC(RWindowGroup* aWg)
	{
	CContainerHider* self = new (ELeave) CContainerHider();
	CleanupStack::PushL(self);
	self->ConstructL(aWg);
	return self;
	}

CContainerHider* CContainerHider::NewL(RWindowGroup* aWg)
	{
	CContainerHider* self = CContainerHider::NewLC(aWg);
	CleanupStack::Pop(); // self;
	return self;
	}

void CContainerHider::ConstructL(RWindowGroup* aWg)
	{
	CreateWindowL(aWg);
	InitComponentArrayL();
	TRgb col(0,0,0,0);
	Window().SetTransparencyAlphaChannel();
	Window().SetBackgroundColor(col);
	MakeVisible(EFalse);
	SetExtentToWholeScreen();
	ActivateL();
	}
void CContainerHider::Draw(const TRect& aRect) const
	{
	CWindowGc& gc=SystemGc();
	gc.Clear();
	}
void CContainerHider::HandlePointerEventL(const TPointerEvent& aEvent)
	{
#ifndef _DEBUG
	if (aEvent.iType==aEvent.EButton1Down)
		{
		_LOG(_L("Hide by tap"));
		((CquickLauncherAppUi*)CEikonEnv::Static()->AppUi())->ShowContainer(EFalse,ETrue);
		}
#endif
	}




CContainerButton::CContainerButton():CCoeControl()
	{
	// No implementation required
	}

CContainerButton::~CContainerButton()
	{
	}

CContainerButton* CContainerButton::NewLC(CCoeControl* aWg)
	{
	CContainerButton* self = new (ELeave) CContainerButton();
	CleanupStack::PushL(self);
	self->ConstructL(aWg);
	return self;
	}

CContainerButton* CContainerButton::NewL(CCoeControl* aWg)
	{
	CContainerButton* self = CContainerButton::NewLC(aWg);
	CleanupStack::Pop(); // self;
	return self;
	}

void CContainerButton::ConstructL(CCoeControl* aWg)
	{
	_LOG(_L("CContainerButton::ConstructL"));
	SetContainerWindowL(*aWg);
	TRgb col(0,0,0,0);
	Window().SetTransparencyAlphaChannel();
	Window().SetBackgroundColor(col);
	MakeVisible(EFalse);
	ActivateL();
	}
void CContainerButton::Draw(const TRect& aRect) const
	{
	CWindowGc& gc=SystemGc();
	gc.Clear();
	if (iIcon)
		{gc.BitBltMasked(Position(),iIcon->Bitmap(),TRect(TPoint(0,0),iIcon->Bitmap()->SizeInPixels()),iIcon->Mask(),EFalse);}
	}
void CContainerButton::SetIcon(CGulIcon* aIcon)
	{
	_LOGDATA(_L("CContainerButton::SetIcon %d"),aIcon);
	iIcon=aIcon;
	}
void CContainerButton::SetObserver(MCoeControlObserver* aObserver)
	{
	_LOGDATA(_L("CContainerButton::SetObserver %d"),aObserver);
	iObserver=aObserver;
	}
/*
TKeyResponse CAppContainer::OfferKeyEventL(const TKeyEvent& aKey,enum TEventCode aType)
	{
	return EKeyWasNotConsumed;
	}
*/
void CAppContainer::CopyBitmapL(CFbsBitmap *aSource, CFbsBitmap *aTarget)
	{
	if(aSource != NULL && aTarget != NULL)
		{
		if(aSource->SizeInPixels() != aTarget->SizeInPixels() || aSource->DisplayMode() != aTarget->DisplayMode())
			{User::Leave(KErrArgument);}
		CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(aTarget);
		CleanupStack::PushL(device);
		CFbsBitGc* gc = NULL;
		User::LeaveIfError(device->CreateContext(gc));
		CleanupStack::PushL(gc);
		gc->BitBlt(TPoint(0, 0), aSource);
		CleanupStack::PopAndDestroy(gc);
		CleanupStack::PopAndDestroy(device);
		}
	}
