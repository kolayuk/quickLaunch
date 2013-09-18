/*
 ============================================================================
 Name		: OVIUtils.h
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2011

 Description : COVIUtils declaration
 ============================================================================
 */

#ifndef OVIUTILS_H
#define OVIUTILS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


//SYSTEM INCLUDES
#include <coecntrl.h>
#include <AknsDrawUtils.h>
#include <AknsBasicBackgroundControlContext.h>
#include <fbs.h>
#include <akndialog.h>

//CONSTANTS
#ifdef __WINS__
_LIT(KImageFileName,"z:\\resource\\apps\\quickLauncher_aif.mif");
#else
_LIT(KImageFileName,"c:\\resource\\apps\\quickLauncher_aif.mif");
#endif

_LIT(KOurPageInOvi, "http://store.ovi.mobi/publisher/Alexander+Fokin");


//CLASS DECLARATION

namespace Utils
{
	void CompleteWithAppPathM(TDes &aPath);
	void OpenBrowser(const TDesC& aUrl);
}


class CAboutDialog : public CAknDialog
    {
    public:
    	CAboutDialog();
        static TInt RunDlgLD();
        void PostLayoutDynInitL();
        TBool OkToExitL(TInt aButtonID);
    private:
        SEikControlInfo CreateCustomControlL(TInt aControlType);
    };

class CAboutContainer : public CCoeControl
{
public:
	void ConstructL(const TRect& aRect);
	CAboutContainer();
	~CAboutContainer();
	void HandleResourceChange(TInt aType);

private:
	void SizeChanged();
	TInt CountComponentControls() const;
	void ConstructFromResourceL(TResourceReader& aReader);
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Draw(const TRect& aRect) const;
	void DrawStringCentered(const CFont *font, const TDesC &string, TInt& top, TInt width, CWindowGc& gc) const;
	void SetIconSize(const TRect& aRect);

private: //data
	CAknsBasicBackgroundControlContext* iBgContext;
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
	CFbsBitmap *iBitmap;
	CFbsBitmap *iMask;
};


class OVIUtils
	{
public:
	static void GoToOvi();
	static void AboutDialog();
	};

#endif // OVIUTILS_H
