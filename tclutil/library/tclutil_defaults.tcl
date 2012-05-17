# E.S.O. - VLT project
# "@(#) $Id: tclutil_defaults.tcl,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"
#
# defaults.tcl - set widget defaults
#
# who         when       what
# --------   ---------   ----------------------------------------------
# A.Brighton 11 Oct 95   created
# P.W.Draper 18 Mar 2012 Comment out text bindings, these are broken.

# set general widget defaults

proc util::setXdefaults {} {
    ::global tcl_version

    option add *foreground Black 
    option add *background #B2B2B2 
    option add *DisabledForeground Gray90

    set labelFont TkDefaultFont
    option add *Font $labelFont

    option add *Menu.tearOff false

    option add *Chooser.font TkFixedFont
    option add *Listbox.font TkFixedFont
    option add *Entry.font TkFixedFont
    option add *titleFont TkDefaultFont
    option add *Text.Font TkFixedFont

    option add *Text.foreground Black 
    option add *Listbox.foreground Black 
    option add *Entry.foreground Black 

    option add *textBackground #B2B2B2
    option add *selectForeground #C0C0C0 
    option add *selectBackground Black 
    
    option add *highlightThickness 0 
    option add *Text.highlightThickness 2  
    option add *Entry.highlightThickness 2 
    option add *highlightBackground #B2B2B2
    option add *highlightColor Black 

    option add *shelp*Background  #B2B2B2
    option add *shelp*Font TkDefaultFont

    option add *plotBackground #C0C0C0 

    # itk widget defaults (normally you'd put these in the itk sources, but
    # putting them here makes it easier to "pre-load" the classes for the single
    # binary version...

    option add *DialogWidget.messageWidth 4i 
    option add *DialogWidget.justify left 
    option add *DialogWidget.messageFont TkDefaultFont

    option add *InputDialog.messageWidth 3i 
    option add *InputDialog.justify left 
    option add *InputDialog.messageFont TkDefaultFont

    option add *ChoiceDialog.messageWidth 3i 
    option add *ChoiceDialog.justify left 
    option add *ChoiceDialog.messageFont TkDefaultFont

    option add *LabelCheck.anchor w 
    option add *LabelChoice.anchor w 

    option add *LabelEntry.anchor w 

    option add *LabelMenu.anchor w 
    option add *LabelMenu.indicatorOn 1 

    option add *LabelMessage.anchor w 
    option add *LabelMessage.justify left 
    option add *LabelMessage.aspect 2000 

    option add *LabelNumber.anchor w 
    option add *LabelNumber.groove ridge 
    option add *LabelNumber.justify right 

    option add *LabelValue.anchor w 
    option add *LabelValue.relief ridge 

    option add *LabelWidget.anchor w 
    option add *LabelWidget.labelFont TkDefaultFont

    option add *ListboxWidget.relief sunken 
    option add *ListboxWidget.borderwidth 3 
    option add *ListboxWidget.setgrid 1 
    option add *ListboxWidget.font TkFixedFont
    option add *ListboxWidget.titleFont TkDefaultFont

    option add *ProgressBar.font TkDefaultFont
    option add *ProgressBar.width 8 
    option add *ProgressBar.length 6c 
    option add *ProgressBar.readonlyBackground {}

    option add *TableList.relief sunken 
    option add *TableList.borderwidth 3 
    option add *TableList.font TkFixedFont
    option add *TableList.headingFont TkDefaultFont
    option add *TableList.headingLines 1 
    option add *TableList.titleFont TkDefaultFont

    option add *TextDialog.textWidth 80 
    option add *TextDialog.textHeight 40 
    option add *TextDialog.messageWidth 3i 
    option add *TextDialog.justify left 
    option add *TextDialog.messageFont TkDefaultFont
    option add *TextDialog.textState disabled 

    option add *ListDialog.messageWidth 3i 
    option add *ListDialog.messageFont TkDefaultFont

    option add *FileSelect.title "Select File"

    # general bindings

    # make delete and backspace work like they should
    if {$tcl_version > 8.3} {
	# tk8.4+ version
	bind Entry <Delete> {::tk::EntryBackspace %W}
	bind Entry <BackSpace> {::tk::EntryBackspace %W}
        #  APIs are now incompatible. Is this used?
	#bind Text <Delete> {::tk::EntryBackspace %W}
	#bind Text <BackSpace> {::tk::EntryBackspace %W}
	bind Button <Return> { ::tk::ButtonInvoke %W }
	bind Radiobutton <Return> { ::tk::CheckRadioInvoke %W }
	bind Checkbutton <Return> { ::tk::CheckRadioInvoke %W }
    } else {
	# tk8.3 version
	bind Entry <Delete> {tkEntryBackspace %W}
	bind Entry <BackSpace> {tkEntryBackspace %W}
	bind Text <Delete> {tkEntryBackspace %W}
	bind Text <BackSpace> {tkEntryBackspace %W}
	bind Button <Return> { tkButtonInvoke %W }
	bind Radiobutton <Return> { tkCheckRadioInvoke %W }
	bind Checkbutton <Return> { tkCheckRadioInvoke %W }
    }
}

