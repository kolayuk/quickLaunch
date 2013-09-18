/*
 ============================================================================
 Name		: AppContainer.h
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : CAppContainer declaration
 ============================================================================
 */

#ifndef APPCONTAINER_H
#define APPCONTAINER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>
#include <quickLauncher.rsg>
#include <aknsbasicbackgroundcontrolcontext.h>
#include <gulicon.h>

class CContainerHider: public CCoeControl
	{
public:
	~CContainerHider();
	static CContainerHider* NewL(RWindowGroup* aWg);
	static CContainerHider* NewLC(RWindowGroup* aWg);
	void ConstructL(RWindowGroup* aWg);
	CContainerHider();
	void Draw(const TRect& aRect) const;
	void HandlePointerEventL(const TPointerEvent& aEvent);
	};

class CContainerButton: public CCoeControl
	{
public:
	~CContainerButton();
	static CContainerButton* NewL(CCoeControl* aWg);
	static CContainerButton* NewLC(CCoeControl* aWg);
	void ConstructL(CCoeControl* aWg);
	CContainerButton();
	void Draw(const TRect& aRect) const;
	CGulIcon* iIcon;
	void SetIcon(CGulIcon* aIcon);
	void SetObserver(MCoeControlObserver* aObserver);
	MCoeControlObserver* iObserver;
	};


// CLASS DECLARATION
class CAppContainer : public CCoeControl, public MCoeControlObserver
	{
public:
	~CAppContainer();
	static CAppContainer* NewL(RWindowGroup* aWg);
	static CAppContainer* NewLC(RWindowGroup* aWg);
	void AddApp(TUid aUid);
	void Draw(const TRect& aRect) const;
	CAknsBasicBackgroundControlContext* iBgContext;
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
	void SizeChanged();
	void HandlePointerEventL(const TPointerEvent& aEvent);
	void Clear();
	void HandleControlEventL(CCoeControl *aControl, TCoeEvent aEventType);
	void Launch(TUid aUid);
	CContainerHider* iHider;
	CGulIcon* iIcon;
	void LoadIcon();
	CGulIcon* LoadAppIconEasy(TUid aUid);
	CGulIcon* LoadAppIconHard(TUid aUid);
	void CopyBitmapL(CFbsBitmap *aSource, CFbsBitmap *aTarget);
	//TKeyResponse OfferKeyEventL(const TKeyEvent& aKey,enum TEventCode aType);
	TInt iDrawSelect;
private:
	CAppContainer();
	void ConstructL(RWindowGroup* aWg);
	RWindowGroup* iWinGroup;
	void CreateButton(TUid aUid);
	};

#endif // APPCONTAINER_H
