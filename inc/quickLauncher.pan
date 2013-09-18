
#ifndef QUICKLAUNCHER_PAN_H
#define QUICKLAUNCHER_PAN_H

/** quickLauncher application panic codes */
enum TquickLauncherPanics
	{
	EquickLauncherUi = 1
	// add further panics here
	};

inline void Panic(TquickLauncherPanics aReason)
	{
	_LIT(applicationName,"quickLauncher");
	User::Panic(applicationName, aReason);
	}

#endif // QUICKLAUNCHER_PAN_H
