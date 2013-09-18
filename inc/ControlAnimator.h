/*
 ============================================================================
 Name		: ControlAnimator.h
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : CControlAnimator declaration
 ============================================================================
 */

#ifndef CONTROLANIMATOR_H
#define CONTROLANIMATOR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

const TInt KStep=35;
const TInt KPeriod=30000;

enum TMoveDirection
	{
	EFromUp,
	EFromDown,
	EFromLeft,
	EFromRight
	};
enum TAnimType
	{
	EShow,
	EHide
	};

// CLASS DECLARATION
class CControlAnimator : public CBase
	{
public:
	~CControlAnimator();
	static CControlAnimator* NewLC(CCoeControl* aControl);
	static CControlAnimator* NewL(CCoeControl* aControl);
	void AnimateShow(TPoint aTarget,TMoveDirection aMoveFrom);
	void AnimateHide(TMoveDirection aMoveFrom);
	static TInt Tick(TAny* aObj);
private:
	CControlAnimator();
	void ConstructL(CCoeControl* aControl);
	CCoeControl* iControl;
	CPeriodic* iTimer;
	void DoTick();
	TPoint iTarget;
	TInt iDx;
	TInt iDy;
	TAnimType iType;
	TMoveDirection iDirection;
	TInt iStep;
	};

#endif // CONTROLANIMATOR_H
