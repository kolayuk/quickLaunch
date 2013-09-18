/*
 ============================================================================
 Name		: ControlAnimator.cpp
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : CControlAnimator implementation
 ============================================================================
 */

#include "ControlAnimator.h"
#include <eikenv.h>
CControlAnimator::CControlAnimator()
	{
	// No implementation required
	}

CControlAnimator::~CControlAnimator()
	{
	}

CControlAnimator* CControlAnimator::NewLC(CCoeControl* aControl)
	{
	CControlAnimator* self = new (ELeave) CControlAnimator();
	CleanupStack::PushL(self);
	self->ConstructL(aControl);
	return self;
	}

CControlAnimator* CControlAnimator::NewL(CCoeControl* aControl)
	{
	CControlAnimator* self = CControlAnimator::NewLC(aControl);
	CleanupStack::Pop(); // self;
	return self;
	}

void CControlAnimator::ConstructL(CCoeControl* aControl)
	{
	iControl=aControl;
	iTimer=CPeriodic::NewL(CActive::EPriorityHigh);
	}
TInt CControlAnimator::Tick(TAny* aObj)
	{
	((CControlAnimator*)aObj)->DoTick();
	return 1;
	}
void CControlAnimator::AnimateShow(TPoint aTarget,enum TMoveDirection aDir)
	{
	if (iTimer->IsActive()){iTimer->Cancel();}
	iType=EShow;
	iDirection=aDir;
	iTarget=aTarget;
	TSize size=iControl->Size();
	TPoint source;
	if (aDir==EFromUp)
		{
		source=TPoint(aTarget.iX,-size.iHeight);
		iDy=KStep; iDx=0;
		}
	else if (aDir==EFromDown)
		{
		source=TPoint(aTarget.iX,CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight+size.iHeight);
		iDy=-KStep; iDx=0;
		}
	else if (aDir==EFromLeft)
		{
		source=TPoint(-size.iWidth,aTarget.iY);
		iDy=0; iDx=KStep;
		}
	else if (aDir==EFromRight)
		{
		source=TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth+size.iWidth,aTarget.iY);
		iDy=0; iDx=-KStep;
		}
	iControl->SetPosition(source);
	iControl->MakeVisible(ETrue);
	iTimer->Start(KPeriod,KPeriod,TCallBack(Tick,this));
	}
void CControlAnimator::AnimateHide(TMoveDirection aDir)
	{
	if (iTimer->IsActive()){iTimer->Cancel();}
	iType=EHide;
	iDirection=aDir;
	TSize size=iControl->Size();
	
	TPoint aSource=iControl->Position();
	TPoint Target;
	if (aDir==EFromUp)
		{
		Target=TPoint(aSource.iX,-size.iHeight);
		iDy=-KStep; iDx=0;
		}
	else if (aDir==EFromDown)
		{
		Target=TPoint(aSource.iX,CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight+size.iHeight);
		iDy=KStep; iDx=0;
		}
	else if (aDir==EFromLeft)
		{
		Target=TPoint(-size.iWidth,aSource.iY);
		iDy=0; iDx=-KStep;
		}
	else if (aDir==EFromRight)
		{
		Target=TPoint(CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth+size.iWidth,aSource.iY);
		iDy=0; iDx=KStep;
		}
	iTarget=Target;
	iTimer->Start(KPeriod,KPeriod,TCallBack(Tick,this));
	}

void CControlAnimator::DoTick()
	{
	TPoint pos=iControl->Position();
	TBool moveok=EFalse;
	if (iDirection==EFromDown||iDirection==EFromUp)
		{moveok=(Abs(iTarget.iY-pos.iY)<KStep);}
	if (iDirection==EFromLeft||iDirection==EFromRight)
		{moveok=(Abs(iTarget.iX-pos.iX)<KStep);}
	if (moveok)
		{
		if (iTimer->IsActive())iTimer->Cancel();
		if (iType==EHide){iControl->MakeVisible(EFalse);}
		else if (iType==EShow){iControl->MakeVisible(ETrue);}
		iControl->SetPosition(iTarget);
		iControl->DrawNow();
		return;
		}
	iControl->SetPosition(TPoint(pos.iX+iDx,pos.iY+iDy));
	iControl->DrawNow();
	}
