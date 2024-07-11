# v1.8.2
    - Updated Dutch Translations
    - Updated Indonesian Translations.
    - Removed "No Transition" as it's not required anymore.
    - Disabled Solid Wave Trail for Mac as it caused issues.
    - (hopefully) Fixed both M1 Mac and Android32 issues.
# v1.8.1 
    - Added Mac Support
    - Added "No Rotate" to the cheat list
    - Updated Turkish Translations
    - Updated Vietnamese Translations
    - Updated Spanish Translations
    - Updated Ukrainian Translations
    - Updated Russian Translations
    - Revert "Fixed bug with the Prism Button bugging out if you switch between windowed and full screen"
# v1.8.0
    - Added "Hide Level"
    - Added "Hide Player"
    - Added "No Rotate"
    - Added importing functionality back
    - "Noclip Accuracy" now only shows if Noclip is enabled
    - Allowed the next frame button to work even at the end of the frame (Macro Editor)
    - Fixed bug with the Prism Button bugging out if you switch between windowed and full screen
    - Fixed a bug with the previous frame button going to the negatives on Android
    - Added a geode setting to "Force Hide Button"
    - Added "Custom Object Bypass" for Android
    - (hopefully) Fixed the hold bug when using Macro Editor
    - (hopefully) Fixed the ImGui font issue
# v1.7.4
    - Update to support Geode Beta 1
# v1.7.3
    - Update Russian translations 
    - Update Turkish translations
    - Update Vietnamese translations
# v1.7.2 
    - Update to support Geode Alpha 2
    - Change about.md
# v1.7.1 
    - Fixed Noclip Accuracy for all platforms
    - (hopefully) Fix the crash issue if someone has an old Prism Menu instance
# v1.7.0 
    - Allowed changing the keybind
    - Added Ukrainian translations (Thank you Lancelot!)
    - Added CBF warning 
    - Added Custom Object Bypass (Only on Windows)
    - Fixed Noclip Accuracy from going to the negatives. (On Windows)
    - Temporarily removed No Pulse
    - Removed Anticheat Bypass (since its useless now)
    - (hopefully) Fix issues with Android on playing back a macro.
    - Changed Mod ID from firee.PrismMenu to firee.prism
# v1.6.4 
    - Added indicators for the hold/release button
    - Added Practice Music Hack back (a silly mistake I made with the if condition)
    - Fix last frame not recalculated after you finish recording.
    - (hopefully) Fix the issues with Macro Editor frame buttons not working
# v1.6.3
    - Fixed issue where macros with no input would crash.
    - Fixed issue with Uncomplete Level not working in ImGui style.
    - Updated Turkish Translations.
    - Updated Russian Translations.
    - Removed ImGui support for Android (because it's not good!)
    - Disabled Practice Music Hack as it caused some things to be unlocked that shouldn't have been unlocked.
    - Fixed issue with deleting a macro not unselecting the deleted macro.
    - Fixed issue with macros crashing the game if the last input frame is less than the real last input frame.
# v1.6.2
    - Removed loadFromCheckpoint hook that caused Mac OS to crash (sorry Mac users :<)
# v1.6.1
    - Fixed Macro Author name
# v1.6.0
    - Added Quartz Bot (A replay bot)
    - Added TPS Bypass (for all platforms)
    - Allowed Noclip Accuracy to no longer require Anticheat Bypass to be enabled.
    - Added No Death Effect.
    - Added Instant Respawn.
    - Added No Wave Pulse.
    - Added Noclip Flash.
    - Added Wave Pulse Size.
    - Changed Info Button to be colored based on the theme.
    - Removed "Enable Patching".
    - Removed "Transparent BG". (Was causing lag, also there are alternative mods available)
    - Refactored hack json files.
    - Fixed No Death Effect not working with Hitbox on Death.
    - Fixed startpos showing progress (again)
# v1.5.1
    - Made the Prism Button hidden if you are in the Editor.
    - Fixed bug with progress saving even in Test Mode.
    - Fixed the ordering of the buttons for "Uncomplete Level".
    - Fixed bug with Prism Button not working when using "Uncomplete Level".
    - Updated Portuguese Translations.
    - Updated Czech Translations.
    - Fixed No Spikes.
    - Fixed No Solids.
# v1.5.0
    - Added Auto Safe Mode.
    - Added No Shaders.
    - Added Uncomplete Level.
    - Allowed changing "percent digits".
    - Added Show Hitboxes.
    - Added Hitbox Stroke.
    - Added Show Hitboxes on Death.
    - Added Suicide.
    - Added Disable Camera Effects.
    - Added Turkish Translations.
    - Added Vietnamese Translations.
    - Fixed "Solid Wave Trail" not working for Mac OS.
    - Fixed "No Transition" not working for Mac OS.
    - Fixed bug with Jump Hack not working when upside down.
    - Fixed issue with "Show Graphic Options" not working on ImGui.
    - Fixed issue with Accurate Percentage not working sometimes in Test Mode.
    - Fixed issue where if you erase all numbers in an input, it will crash on Android.
# v1.4.2
    - Fixed bug with No Mirror Transition & Instant Mirror Portal if "No Effect" is applied.
    - Fixed speedhack not syncing with gameplay correctly.
    - Fixed Platformer Mode buttons not showing if you use Force Platformer Mode on Android.
    - Fixed bug with camera effects affecting Cheat Indicator and Noclip Accuracy.
    - Fixed issue with info text being off screen.
    - Removed Text Length and Character Filter functionality.
    - Fixed issue with Pause Buttons not working on Android after using menu.
    - Fixed Jump Hack from acting on any input.
    - Updated Spanish, Polish, Russian, Czech, and German.
# v1.4.1
    - Fixed Mac OS issue with not being able to open menu.
# v1.4.0
    - Added Dear ImGui back (from request)
    - Fixed Accurate Percentage on levels with the End Level Trigger (Thanks dankmeme01!)
    - Added Menu Style 
    - Added Light Theme
    - Added No Trail
    - Added No Wave Trail
    - Added No Glow
    - Added Layout Mode for Android
    - Added Solid Wave Trail
    - Added Force Platformer Mode & Change Gravity for Android
    - Added Show Hidden Objects (For Layout Mode)
    - Added Noclip Accuracy
    - Added a "Show Graphic Options" button (For Android users)
# v1.3.2 
    - Removed Dear ImGui :(
    - Removed Menu Style.
    - Fixed performance issues.
    - Fixed text centering issues with info popup.
# v1.3.1 
    - Prevented users from being softlocked via speedhack.
    - Fixed text scaling issue with other languages.
    - Fixed issue with big checkmark on low & high quality mode.
    - Disabled FPS Bypass on Mac OS (sorry it just doesn't work)
# v1.3.0 
    - Added Mac OS Support
    - Replaced ImGui with a new natively made UI for GD (mainly for Android users. The option to switch back will be removed in the next update).
    - Added Anticheat Bypass back for Android.
    - Added Speedhack Audio
    - Added Accurate Percentage back (thanks mat!)
    - Fixed Safe Mode
    - Added Spanish Translations (Thank you Electrify, Hero & ItzLucasOG!)
    - Added Polish Translations (Thank you olaf_294 & KONDIROTSU!)
    - Added Malay Translations (Thank you hori!)
    - Added Text Length & Character Filter back.
    - Fixed issue where if you were to import a theme that already exists, the game would crash.
# v1.2.2
    - Fixed save issue
# v1.2.1 
    - Emergency Fix
# v1.2.0
    - Added Indonesian Translations (Thank you RzaIX!)
    - Fixed bug with Texture Loader crashing when applying a texture.
    - Fixed bug with pause buttons not working.
# v1.1.5
    - Added Russian Translations (Thank you MegaSa1nt, savvacorgi, and dankmeme01!)
    - Added Czech Translations (Thank you Ignis!)
    - Removed Free Shop Items for now so I can work on making a stats editor (Basically will make that useless)
# v1.1.41
    - Fixed bug when you enter levels, it crashes. (On Android)
# v1.1.4
    - Added support for 2.205 on Android.
    - Fixed issue where the shop crashes.
    - Removed "No Progress Bar" as it's now added in 2.205
    - Added German translations (Thank you huhnmitferrari!)
    - Fixed Portuguese translations
# v1.1.3
    - Removed "Hide Testmode" from being considered a "cheat" in the "Cheat Indicator"
    - Added Portuguese translations (Obrigado Gazonk!)
# v1.1.2
    - Added live update for Menu Scale
    - Increased the size of the scrollbar
    - Fixed lag issues when the menu is open
    - Fixed bug on PC where gravity doesn't work for platformer levels.
    - Hopefully fixed crashing issue for Android if you press any other button in the Main Menu
# v1.1.1
    - Fixed memory leak if you change languages from English.
    - Fixed issue with Hide Testmode crashing.
    - Fixed issue with Misc settings crashing on newer levels.
    - Fixed other bugs
# v1.1.0
    - Added French Translations (Merci Jouca !)
    - Added Menu Scale (for people using Android)
    - Added "X" button in the menu in case Android users can't close the menu.
    - Added "Verify Hack" and "No (C) Mark" back
    - Fixed issue with Prism Button not working on the main menu
    - Fixed issue with speedhack not working on levels that use Time Warp
    - Fixed "Progress Bar Position", "Attempt Opacity", and "No Progress Bar"
    - Removed Hide Attempts
# v1.0.2
    - Fixed bug with the game crashing if you are on a platformer level, and you press pause a couple of times.
# v1.0.1
    - Disabled Layout Mode on Android (for now)
    - Fixed issue with LevelInfoLayer crashing if its your own level and you have Copy Hack enabled.
    - Fixed Force Platformer Mode and Gravity not working on Windows
# v1.0.0
    - Geode public release
