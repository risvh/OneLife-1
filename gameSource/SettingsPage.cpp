#include "SettingsPage.h"


#include "minorGems/game/game.h"
#include "minorGems/game/Font.h"

#include "minorGems/util/SettingsManager.h"
#include "minorGems/util/stringUtils.h"

#include "minorGems/game/game.h"
#include "minorGems/system/Time.h"

#include "musicPlayer.h"
#include "soundBank.h"
#include "objectBank.h"
#include "buttonStyle.h"

#include "DropdownList.h"


extern Font *mainFont;

extern float musicLoudness;

extern bool useDarkMode;


SettingsPage::SettingsPage()
        : mInfoSeeds( mainFont, 542, -278, "?" ),
		  mBackButton( mainFont, -542, -280, translate( "backButton" ) ),
          mEditAccountButton( mainFont, -463, 129, translate( "editAccount" ) ),
          mRestartButton( mainFont, 128, 128, translate( "restartButton" ) ),
          mRedetectButton( mainFont, 153, 249, translate( "redetectButton" ) ),
          mFullscreenBox( 0, 128, 4 ),
          mBorderlessBox( 0, 168, 4 ),
          mEnableNudeBox( -335, 148, 4 ),
		  mUseDarkModeBox( 0, -278, 4 ),
          mMusicLoudnessSlider( mainFont, 0, 40, 4, 200, 30,
                                0.0, 1.0, 
                                translate( "musicLoudness" ) ),
          mSoundEffectsLoudnessSlider( mainFont, 0, -48, 4, 200, 30,
                                       0.0, 1.0, 
                                       translate( "soundLoudness" ) ),
          mUseCustomServerBox( -168, -148, 4 ),
          mCustomServerAddressField( mainFont, 306, -150, 14, false,
                                     translate( "address" ),
                                     NULL,
                                     // forbid spaces
                                     " " ),
          mCustomServerPortField( mainFont, 84, -208, 4, false,
                                  translate( "port" ),
                                  "0123456789", NULL ),
          mCopyButton( mainFont, 381, -216, translate( "copy" ) ),
          mPasteButton( mainFont, 518, -216, translate( "paste" ) ),
          mSpawnSeed( mainFont, 226, -278, 14, false, 
                                     translate( "spawnSeed" ),
                                     NULL,
                                     // forbid spaces
                                     " " ),
          mCursorScaleSlider( mainFont, 297, 155, 4, 200, 30,
                                       1.0, 10.0, 
                                       translate( "scale" ) ) {
                            

    
    const char *choiceList[3] = { translate( "system" ),
                                  translate( "drawn" ),
                                  translate( "both" ) };
    
    mCursorModeSet = 
        new RadioButtonSet( mainFont, 561, 275,
                            3, choiceList,
                            false, 4 );
    addComponent( mCursorModeSet );
    mCursorModeSet->addActionListener( this );

    addComponent( &mCursorScaleSlider );
    mCursorScaleSlider.addActionListener( this );

    mCursorScaleSlider.toggleField( false );


	setButtonStyle( &mInfoSeeds );
    setButtonStyle( &mBackButton );
    setButtonStyle( &mEditAccountButton );
    setButtonStyle( &mRestartButton );
    setButtonStyle( &mRedetectButton );
    setButtonStyle( &mCopyButton );
    setButtonStyle( &mPasteButton );

	addComponent( &mInfoSeeds);
	mInfoSeeds.addActionListener( this );

    addComponent( &mBackButton );
    mBackButton.addActionListener( this );

    addComponent( &mEditAccountButton );
    mEditAccountButton.addActionListener( this );

    addComponent( &mFullscreenBox );
    mFullscreenBox.addActionListener( this );

    addComponent( &mBorderlessBox );
    mBorderlessBox.addActionListener( this );

    addComponent( &mEnableNudeBox );
    mEnableNudeBox.addActionListener( this );
	
    addComponent( &mUseDarkModeBox );
    mUseDarkModeBox.addActionListener( this );

    addComponent( &mRestartButton );
    mRestartButton.addActionListener( this );
    
    addComponent( &mRedetectButton );
    mRedetectButton.addActionListener( this );

    addComponent( &mUseCustomServerBox );
    addComponent( &mCustomServerAddressField );
    addComponent( &mCustomServerPortField );
    
	mCustomServerAddressField.usePasteShortcut( true );
	
    // addComponent( &mCopyButton );
    // addComponent( &mPasteButton );
    
    mCopyButton.addActionListener( this );
    mPasteButton.addActionListener( this );
    
    if( ! isClipboardSupported() ) {
        mCopyButton.setVisible( false );
        mPasteButton.setVisible( false );
        }

    addComponent( &mSpawnSeed );
	
	mSpawnSeed.usePasteShortcut( true );
    
    mRestartButton.setVisible( false );
    
    mOldFullscreenSetting = 
        SettingsManager::getIntSetting( "fullscreen", 1 );
    
    mTestSound = blankSoundUsage;

    mMusicStartTime = 0;

    mFullscreenBox.setToggled( mOldFullscreenSetting );

    mBorderlessBox.setVisible( mOldFullscreenSetting );


    mOldBorderlessSetting = 
        SettingsManager::getIntSetting( "borderless", 0 );

    mBorderlessBox.setToggled( mOldBorderlessSetting );

    mEnableNudeSetting =
        SettingsManager::getIntSetting( "nudeEnabled", 1 );

    mEnableNudeBox.setToggled( mEnableNudeSetting );
	
    mUseDarkModeSetting =
        SettingsManager::getIntSetting( "useDarkMode", 0 );

    mUseDarkModeBox.setToggled( mUseDarkModeSetting );
	
	mOldUseDarkModeSetting = mUseDarkModeSetting;
    
    

    addComponent( &mMusicLoudnessSlider );
    addComponent( &mSoundEffectsLoudnessSlider );
    
    mMusicLoudnessSlider.addActionListener( this );
    mSoundEffectsLoudnessSlider.addActionListener( this );

    }



SettingsPage::~SettingsPage() {
    clearSoundUsage( &mTestSound );

    delete mCursorModeSet;
    }




void SettingsPage::actionPerformed( GUIComponent *inTarget ) {
    if( inTarget == &mBackButton ) {
			
		char *seedList = mSpawnSeed.getAndUpdateList();
		SettingsManager::setSetting( "spawnSeed", seedList );
		delete [] seedList;
		
		int useCustomServer = 0;
		if( mUseCustomServerBox.getToggled() ) {
			useCustomServer = 1;
			}
		
		SettingsManager::setSetting( "useCustomServer", useCustomServer );
		char *address = mCustomServerAddressField.getText();
		
		SettingsManager::setSetting( "customServerAddress", address );
		delete [] address;
		
		SettingsManager::setSetting( "customServerPort",
									 mCustomServerPortField.getInt() );
        
        setSignal( "back" );
        setMusicLoudness( 0 );
        }
	else if( inTarget == &mInfoSeeds ) {
		 char *url = strdup("https://twohoursonelife.fandom.com/wiki/Spawn_seeds");
		 
		 if( strcmp( url, "" ) != 0 ) {
			 launchURL( url );
			}
		}
    else if( inTarget == &mEditAccountButton ) {
        
        setSignal( "editAccount" );
        setMusicLoudness( 0 );
        }
    else if( inTarget == &mFullscreenBox ) {
        int newSetting = mFullscreenBox.getToggled();
        
        SettingsManager::setSetting( "fullscreen", newSetting );
        
        mRestartButton.setVisible( mOldFullscreenSetting != newSetting );
        if ( useDarkMode ) mRestartButton.setPosition( 182, mFullscreenBox.getPosition().y );
		
        mBorderlessBox.setVisible( newSetting );
        }
    else if( inTarget == &mBorderlessBox ) {
        int newSetting = mBorderlessBox.getToggled();
        
        SettingsManager::setSetting( "borderless", newSetting );
        
        mRestartButton.setVisible( mOldBorderlessSetting != newSetting );
        if ( useDarkMode ) mRestartButton.setPosition( 182, mBorderlessBox.getPosition().y );
        }
	else if( inTarget == &mEnableNudeBox ) {
        int newSetting = mEnableNudeBox.getToggled();
        
        SettingsManager::setSetting( "nudeEnabled", newSetting );
        
        mRestartButton.setVisible( mEnableNudeSetting != newSetting );
        if ( useDarkMode ) mRestartButton.setPosition( 182, mEnableNudeBox.getPosition().y );
        }
	else if( inTarget == &mUseDarkModeBox ) {
        int newSetting = mUseDarkModeBox.getToggled();
        
        SettingsManager::setSetting( "useDarkMode", newSetting );
		
		useDarkMode = newSetting;
        }
    else if( inTarget == &mRestartButton ||
             inTarget == &mRedetectButton ) {
        // always re-measure frame rate after relaunch
        SettingsManager::setSetting( "targetFrameRate", -1 );
        SettingsManager::setSetting( "countingOnVsync", -1 );
        
        char relaunched = relaunchGame();
        
        if( !relaunched ) {
            printf( "Relaunch failed\n" );
            setSignal( "relaunchFailed" );
            }
        else {
            printf( "Relaunched... but did not exit?\n" );
            setSignal( "relaunchFailed" );
            }
        }
    else if( inTarget == &mSoundEffectsLoudnessSlider ) {
        setMusicLoudness( 0 );
        mMusicStartTime = 0;
        
        if( ! mSoundEffectsLoudnessSlider.isPointerDown() ) {
            
            setSoundEffectsLoudness( 
                mSoundEffectsLoudnessSlider.getValue() );
        
            if( mTestSound.numSubSounds > 0 ) {
                doublePair pos = { 0, 0 };
                
                playSound( mTestSound, pos );
                }
            }
        }    
    else if( inTarget == &mMusicLoudnessSlider ) {
            

        if( ! mSoundEffectsLoudnessSlider.isPointerDown() ) {
            musicLoudness = mMusicLoudnessSlider.getValue();
            SettingsManager::setSetting( "musicLoudness", musicLoudness );
            }
            
        
        if( Time::getCurrentTime() - mMusicStartTime > 25 ) {

            instantStopMusic();
            

            restartMusic( 9.0, 1.0/60.0, true );
            
            setMusicLoudness( mMusicLoudnessSlider.getValue(), true );


            mMusicStartTime = Time::getCurrentTime();
            }
        else {
            setMusicLoudness( mMusicLoudnessSlider.getValue(), true );
            }
        }
    else if( inTarget == &mCopyButton ) {
        char *address = mCustomServerAddressField.getText();
        
        char *fullAddress = autoSprintf( "%s:%d", address,
                                         mCustomServerPortField.getInt() );
        delete [] address;
        
        setClipboardText( fullAddress );
        
        delete [] fullAddress;
        }
    else if( inTarget == &mPasteButton ) {
        char *text = getClipboardText();

        char *trimmed = trimWhitespace( text );
        
        delete [] text;
        

        char setWithPort = false;
        
        if( strstr( trimmed, ":" ) != NULL ) {
            char addressBuff[100];
            int port = 0;
            
            int numRead = sscanf( trimmed, "%99[^:]:%d", addressBuff, &port );
            
            if( numRead == 2 ) {
                setWithPort = true;
                
                char *trimmedAddr = trimWhitespace( addressBuff );
                
                // terminate at first space, if any
                char *spacePos = strstr( trimmedAddr, " " );
                if( spacePos != NULL ) {
                    spacePos[0] = '\0';
                    }

                mCustomServerAddressField.setText( trimmedAddr );

                delete [] trimmedAddr;
                
                mCustomServerPortField.setInt( port );
                }
            }
        
        if( ! setWithPort ) {
            // treat the whole thing as an address
            
            // terminate at first space, if any
            char *spacePos = strstr( trimmed, " " );
            
            if( spacePos != NULL ) {
                spacePos[0] = '\0';
                }
            mCustomServerAddressField.setText( trimmed );
            }
        delete [] trimmed;
        }
    else if( inTarget == mCursorModeSet ) {
        setCursorMode( mCursorModeSet->getSelectedItem() );
        
        int mode = getCursorMode();
        
        mCursorScaleSlider.setVisible( mode > 0 );
        
        mCursorScaleSlider.setValue( getEmulatedCursorScale() );
        }
    else if( inTarget == &mCursorScaleSlider ) {
        setEmulatedCursorScale( mCursorScaleSlider.getValue() );
        }
    
    
    }


extern int targetFramesPerSecond;


void SettingsPage::draw( doublePair inViewCenter, 
                         double inViewSize ) {	
	
	if ( useDarkMode ) {
		darkModeDraw( inViewCenter, inViewSize );
		return;
	}
	
	setDrawColor( 1, 1, 1, 1 );
    
    doublePair pos = mFullscreenBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;
    
    mainFont->drawString( translate( "fullscreen" ), pos, alignRight );
    

    if( mBorderlessBox.isVisible() ) {
        pos = mBorderlessBox.getPosition();
    
        pos.x -= 30;
        pos.y -= 2;
        
        mainFont->drawString( translate( "borderless" ), pos, alignRight );
        }
    

    pos = mFullscreenBox.getPosition();
    
    pos.y += 96;
    pos.x -= 16;
    
    if( getCountingOnVsync() ) {
        mainFont->drawString( translate( "vsyncYes" ), pos, alignLeft );
        }
    else {
        mainFont->drawString( translate( "vsyncNo" ), pos, alignLeft );
        }
    
    pos.y += 44;

    char *fpsString = autoSprintf( "%d", targetFramesPerSecond );
    
    mainFont->drawString( fpsString, pos, alignLeft );
    delete [] fpsString;


    pos.y += 44;

    char *currentFPSString = autoSprintf( "%.2f", getRecentFrameRate() );
    
    mainFont->drawString( currentFPSString, pos, alignLeft );
    delete [] currentFPSString;
    

    pos = mFullscreenBox.getPosition();
    pos.x -= 30;

    pos.y += 96;
    mainFont->drawString( translate( "vsyncOn" ), pos, alignRight );
    pos.y += 44;
    mainFont->drawString( translate( "targetFPS" ), pos, alignRight );
    pos.y += 44;
    mainFont->drawString( translate( "currentFPS" ), pos, alignRight );


    pos = mEnableNudeBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;

    mainFont->drawString( "Enable Nudity", pos, alignRight );
	
    pos = mUseDarkModeBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;

    mainFont->drawString( "USE DARK MODE", pos, alignRight );


    pos = mCursorModeSet->getPosition();
    
    pos.y += 37;
    mainFont->drawString( translate( "cursor"), pos, alignRight );
    
    if( mCursorScaleSlider.isVisible() ) {
        
        pos = mCursorScaleSlider.getPosition();
        
        pos.x += 72;
        pos.y -= 2;
        
        mainFont->drawString( translate( "scale"), pos, alignRight );
        }
    
    }



void SettingsPage::step() {
    if( mTestSound.numSubSounds > 0 ) {
        markSoundUsageLive( mTestSound );
        }
    stepMusicPlayer();
	
	if ( mOldUseDarkModeSetting != useDarkMode ) 
		updateOnDarkMode();
    }





void SettingsPage::makeActive( char inFresh ) {
    if( inFresh ) {        

        int mode = getCursorMode();
        
        mCursorModeSet->setSelectedItem( mode );
        
        mCursorScaleSlider.setVisible( mode > 0 );
        
        mCursorScaleSlider.setValue( getEmulatedCursorScale() );

		char *seedList = SettingsManager::getSettingContents( "spawnSeed", "" );
		mSpawnSeed.setList( seedList );
		delete [] seedList;

			
		int useCustomServer = 
			SettingsManager::getIntSetting( "useCustomServer", 0 );
		
		mUseCustomServerBox.setToggled( useCustomServer );
		

		char *address = 
			SettingsManager::getStringSetting( "customServerAddress",
											   "localhost" );
		
		int port = SettingsManager::getIntSetting( "customServerPort", 8005 );
		
		mCustomServerAddressField.setText( address );
		mCustomServerPortField.setInt( port );
		
		delete [] address;
        

        mMusicLoudnessSlider.setValue( musicLoudness );
        mSoundEffectsLoudnessSlider.setValue( getSoundEffectsLoudness() );
        setMusicLoudness( 0 );
        mMusicStartTime = 0;
        
        int tryCount = 0;
        
        while( mTestSound.numSubSounds == 0 && tryCount < 10 ) {

            int oID = getRandomPersonObject();

            if( oID > 0 ) {
                ObjectRecord *r = getObject( oID );
                if( r->usingSound.numSubSounds >= 1 ) {
                    mTestSound = copyUsage( r->usingSound );
                    // constrain to only first subsound                    
                    mTestSound.numSubSounds = 1;
                    // play it at full volume
                    mTestSound.volumes[0] = 1.0;
                    }
                }
            tryCount ++;
            }
        
        if( SettingsManager::getIntSetting( "useSteamUpdate", 0 ) ) {
            mEditAccountButton.setVisible( true );
            }
        else {
            mEditAccountButton.setVisible( false );
            }

        }
		
	updateOnDarkMode();
    }



void SettingsPage::makeNotActive() {
    }
	
void SettingsPage::updateOnDarkMode() {

	if ( useDarkMode ) {
		mMusicLoudnessSlider.setPosition( 0, 300 );
		mSoundEffectsLoudnessSlider.setPosition( 0, 248 );
		mRedetectButton.setPosition( 182, 198 );
		mFullscreenBox.setPosition( -38, 40 );
		mBorderlessBox.setPosition( -38, -20 );
		mEnableNudeBox.setPosition( -38, -80 );
		mUseCustomServerBox.setVisible( true );
		mCustomServerAddressField.setVisible( mUseCustomServerBox.getToggled() );
		mCustomServerPortField.setVisible( mUseCustomServerBox.getToggled() );
		mUseCustomServerBox.setPosition( -38, -200 );
		mCustomServerAddressField.setPosition( 456, -202 );
		mCustomServerPortField.setPosition( 234, -262 );
		mSpawnSeed.setVisible( false );
		mCursorModeSet->setVisible( false );
		mInfoSeeds.setVisible( false );
		mEditAccountButton.setVisible( false );
		mCopyButton.setVisible( false );
		mPasteButton.setVisible( false );
		
		int y = mFullscreenBox.getPosition().y;
		if( mEnableNudeSetting != mEnableNudeBox.getToggled() ) y = mEnableNudeBox.getPosition().y;		
		if( mOldBorderlessSetting != mBorderlessBox.getToggled() ) y = mBorderlessBox.getPosition().y;
		if( mOldFullscreenSetting != mFullscreenBox.getToggled() ) y = mFullscreenBox.getPosition().y;
		mRestartButton.setPosition( 182, y );
		
		mUseDarkModeBox.setPosition( -38, -140 );
		mOldUseDarkModeSetting = true;
	} else {
		mMusicLoudnessSlider.setPosition( 0, 40 );
		mSoundEffectsLoudnessSlider.setPosition( 0, -48 );
		mRedetectButton.setPosition( 153, 249 );
		mFullscreenBox.setPosition( 0, 128 );
		mBorderlessBox.setPosition( 0, 168 );
		mEnableNudeBox.setPosition( 0, -150 );
		mUseCustomServerBox.setVisible( false );
		mCustomServerAddressField.setVisible( false );
		mCustomServerPortField.setVisible( false );
		mUseCustomServerBox.setPosition( -168, -148 );
		mCustomServerAddressField.setPosition( 306, -150 );
		mCustomServerPortField.setPosition( 84, -208 );
		mSpawnSeed.setVisible( !SettingsManager::getIntSetting( "useSteamUpdate", 0 ) );
		mCursorModeSet->setVisible( true );
		mInfoSeeds.setVisible( !SettingsManager::getIntSetting( "useSteamUpdate", 0 ) );
		mEditAccountButton.setVisible( SettingsManager::getIntSetting( "useSteamUpdate", 0 ) );
		mCopyButton.setVisible( true );
		mPasteButton.setVisible( true );
		if( ! isClipboardSupported() ) {
			mCopyButton.setVisible( false );
			mPasteButton.setVisible( false );
			}
			
		mRestartButton.setPosition( 128, 128 );

		mUseDarkModeBox.setPosition( 0, -190 );
		mOldUseDarkModeSetting = false;
	}

}

void SettingsPage::darkModeDraw( doublePair inViewCenter, 
                         double inViewSize ) {
    setDrawColor( 0.5, 0.5, 0.5, 1 );
	
	mCustomServerAddressField.setVisible( mUseCustomServerBox.getToggled() );
	mCustomServerPortField.setVisible( mUseCustomServerBox.getToggled() );
    
    doublePair pos = mFullscreenBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;
    
    mainFont->drawString( translate( "fullscreen" ), pos, alignRight );


    pos = mUseCustomServerBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;
    
    mainFont->drawString( translate( "useCustomServer" ), pos, alignRight );
    

    if( mBorderlessBox.isVisible() ) {
        pos = mBorderlessBox.getPosition();
    
        pos.x -= 30;
        pos.y -= 2;
        
        mainFont->drawString( translate( "borderless" ), pos, alignRight );
        }
    

    pos = mFullscreenBox.getPosition();
    
    pos.y += 52;
    pos.x -= 16;
    
    if( getCountingOnVsync() ) {
        mainFont->drawString( translate( "vsyncYes" ), pos, alignLeft );
        }
    else {
        mainFont->drawString( translate( "vsyncNo" ), pos, alignLeft );
        }
    
    pos.y += 52;

    char *fpsString = autoSprintf( "%d", targetFramesPerSecond );
    
    mainFont->drawString( fpsString, pos, alignLeft );
    delete [] fpsString;


    pos.y += 52;

    char *currentFPSString = autoSprintf( "%.2f", getRecentFrameRate() );
    
    mainFont->drawString( currentFPSString, pos, alignLeft );
    delete [] currentFPSString;
    

    pos = mFullscreenBox.getPosition();
    pos.x -= 30;

    pos.y += 52;
    mainFont->drawString( translate( "vsyncOn" ), pos, alignRight );
    pos.y += 52;
    mainFont->drawString( translate( "targetFPS" ), pos, alignRight );
    pos.y += 52;
    mainFont->drawString( translate( "currentFPS" ), pos, alignRight );


    pos = mEnableNudeBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;

    mainFont->drawString( "Enable Nudity:", pos, alignRight );
	
	
    pos = mUseDarkModeBox.getPosition();
    
    pos.x -= 30;
    pos.y -= 2;

    mainFont->drawString( "USE DARK MODE:", pos, alignRight );


    pos = mCursorModeSet->getPosition();
    
    pos.y += 37;
    // mainFont->drawString( translate( "cursor"), pos, alignRight );
    
    if( mCursorScaleSlider.isVisible() ) {
        
        pos = mCursorScaleSlider.getPosition();
        
        pos.x += 72;
        pos.y -= 2;
        
        mainFont->drawString( translate( "scale"), pos, alignRight );
        }
    
    }