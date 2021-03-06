#RequireAdmin

#include <ButtonConstants.au3>
#include <EditConstants.au3>
#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
Opt("GUIOnEventMode", 1)
#Region ### START Koda GUI section ### Form=
$Form1 = GUICreate("AAL-Helper", 307, 140, 461, 254)
GUISetOnEvent($GUI_EVENT_CLOSE, "Form1Close")
$Button1 = GUICtrlCreateButton("Select File", 8, 72, 91, 25)
GUICtrlSetOnEvent(-1, "Button1Click")
$Input1 = GUICtrlCreateInput("SrcFile.aal1", 8, 8, 289, 21)
$Input2 = GUICtrlCreateInput("Include-Directory", 8, 40, 289, 21)
$Button2 = GUICtrlCreateButton("Developer Guide", 200, 72, 91, 25)
GUICtrlSetOnEvent(-1, "Button2Click")
$Button3 = GUICtrlCreateButton("Run Release", 104, 72, 91, 25)
GUICtrlSetOnEvent(-1, "Button3Click")
$Button6 = GUICtrlCreateButton("Run Debug", 104, 104, 91, 25)
GUICtrlSetOnEvent(-1, "Button6Click")
$Button4 = GUICtrlCreateButton("AAL Doc", 8, 104, 91, 25)
GUICtrlSetOnEvent(-1, "Button4Click")
$Button5 = GUICtrlCreateButton("Source Code Doc", 200, 104, 91, 25)
GUICtrlSetOnEvent(-1, "Button5Click")
GUISetState(@SW_SHOW)
#EndRegion ### END Koda GUI section ###

While 1
	Sleep(100)
WEnd

Func Button1Click()
	$file = FileOpenDialog("File to run", ".", "All (*.aal1)")
	GUICtrlSetData($Input1, $file)

	$split=StringSplit($file,"\",2)
	$path=StringReplace($file,$split[Ubound($split)-1],"")
	GUICtrlSetData($Input2, $path)
EndFunc   ;==>Button1Click

Func Button2Click()
	ShellExecute(".\Getting Started\aal-developer-manual.pdf")
EndFunc   ;==>Button2Click

Func Button3Click()
	$file = GUICtrlRead($Input1)
	$include = GUICtrlRead($Input2)

	FileDelete(@ScriptDir&"\bin\compile.aal1")
	FileDelete(@ScriptDir&"\bin\out.aal1.bin")
	ShellExecuteWait(@ScriptDir&"\bin\compiler_release.bat",'"'&$file&'" "'&@ScriptDir&'\bin\out.aal1.bin" "'&$include&'"',@ScriptDir&"\bin")
	ShellExecuteWait(@ScriptDir&"\bin\interpreter_release.bat","",@ScriptDir&"\bin")
EndFunc   ;==>Button3Click


Func Button6Click()
	$file = GUICtrlRead($Input1)
	$include = GUICtrlRead($Input2)

	FileDelete(@ScriptDir&"\bin\compile.aal1")
	FileDelete(@ScriptDir&"\bin\out.aal1.bin")
	ShellExecuteWait(@ScriptDir&"\bin\compiler_debug.bat",'"'&$file&'" "'&@ScriptDir&'\bin\out.aal1.bin" "'&$include&'"',@ScriptDir&"\bin")
	ShellExecuteWait(@ScriptDir&"\bin\interpreter_debug.bat","",@ScriptDir&"\bin")
EndFunc   ;==>Button6Click

Func Button4Click()
	ShellExecute(".\Getting Started\aal_functions.html")
EndFunc   ;==>Button4Click

Func Button5Click()
	ShellExecute(".\Sourcecode-Doc\index.html")
EndFunc   ;==>Button5Click

Func Form1Close()
	Exit
EndFunc   ;==>Form1Close


