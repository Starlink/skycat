# E.S.O. - VLT project 
# "@(#) $Id: RtdImageHduChooser.tcl,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"
#
# RtdImageHduChooser.tcl - Itcl widget for displaying FITS extensions
#                          (HDU's = Header Data Units)
# 
# See man page RtdImageHduChooser(n) for a complete description.
# 
# who             when       what
# --------------  ---------  ----------------------------------------
# Allan Brighton  09/11/98   Created
# pbiereic        14/08/01   Added default method display_fits_table()
#                            for binary tables.
#                            Display HDU images only on request (for
#                            faster display of main image)
# pbiereic        11/10/08   Break image display loop after failure.
# pbiereic        26/11/08   Using 'view update' for HDU images display.
#                            Using a toplevel window for the HDU images display.

itk::usual RtdImageHduChooser {}

# This class defines a widget for displaying the HDUs in the current FITS
# image. The user can select an image extension to display 
# by clicking on an entry the list or on one of the small images displayed 
# in a table.

itcl::class rtd::RtdImageHduChooser {
    inherit util::TopLevelWidget


    # constructor: create a toplevel window

    constructor {args} {
	eval itk_initialize $args
        wm protocol $w_ WM_DELETE_WINDOW [code $this quit]
    }


    # This method is called after the options have been evaluated.

    protected method init {} {
	wm title $w_ "FITS HDUs ($itk_option(-number))"
	wm iconname $w_ "FITS HDUs ($itk_option(-number))"

	set topf $w_.topf
	itk_component add topf {
	    frame $topf
	}
        pack $itk_component(topf) -fill both -expand 1

	set cmap [$image_ cmap file]
	set itt [$image_ itt file]
	set colorscale [$image_ colorscale]
	set settings_ "$cmap $itt $colorscale";   # save for later comparison

	make_buttons $topf
	make_table $topf
	
	show_hdu_list
    }
    
    # Quit this widget

    public method quit { } {
        destroy $w_
    }

    # add a row of buttons at bottom

    protected method make_buttons {f} {
        global $w_.show
        set $w_.show $itk_option(-show_images)

	# Button frame at bottom of window
	itk_component add buttons {
	    frame $f.buttons -borderwidth 2
	}

	itk_component add open {
	    button $itk_component(buttons).open \
		-text Open \
		-command [code $this set_hdu]
	}
	add_short_help $itk_component(open) \
	    {Open and display the selected HDU image or table}

	itk_component add display {
	    button $itk_component(buttons).display \
		-text "Display as one Image" \
		-state disabled \
		-command [code $this display_as_one_image]
	}
	add_short_help $itk_component(display) \
	    {Display the image extensions as a single image using the CRPIX values from the FITS header}

	itk_component add show {
	    checkbutton $itk_component(buttons).show \
                -text "Show HDU images" \
                -variable $w_.show \
                -onvalue 1 -offvalue 0 \
                -borderwidth 2 \
                -relief raised \
                -state disabled \
                -pady 4 \
                -highlightthickness 0 \
                -command [code $this show_images]
	}
        add_short_help $itk_component(show) \
	    {Show/Hide the display of the miniature versions of the image extensions}

	itk_component add delete {
	    button $itk_component(buttons).delete \
		-text Delete \
		-state disabled \
		-command [code $this delete_hdu]
	}
	add_short_help $itk_component(delete) \
	    {Delete the selected HDU from the FITS file}

	itk_component add close {
	    button $itk_component(buttons).close \
		-text Close \
		-command [code $this quit]
	}
	add_short_help $itk_component(close) {Close the selected HDU}

	pack $itk_component(open) $itk_component(display) $itk_component(show) \
	    $itk_component(delete) $itk_component(close) \
	    -side left -expand 1 -fill x -padx 0.5m -ipadx 0.5m
        
        pack $itk_component(buttons) -side bottom -fill x -expand 0
    }

    
    # Make the table component for displaying the HDU info

    protected method make_table {f} {
	set headings [$image_ hdu listheadings]

	# TableList(n) widget for displaying the list of HDUs
	itk_component add table {
	    util::TableList $f.table \
		-title {FITS HDUs} \
		-headings $headings \
		-width [string length $headings]
	}
	add_short_help $itk_component(table) \
	    {Table: Click to select HDU, double-click to display image or table}

        pack $itk_component(table) -side top -fill both -expand 1

	set listbox [$itk_component(table) component listbox]
	bind $listbox <ButtonRelease-1> [code $this select_hdu]
	bind $listbox <Double-ButtonPress-1> [code $this set_hdu]
    }

    
    # Update the list of HDUs and the image displays, if needed

    public method show_hdu_list {} {
        set old_filename $filename_
        set filename_ [$image_ cget -file]
	set hdu_list [$image_ hdu list]
	set headings [$image_ hdu listheadings]

	# update the table listing
	$itk_component(table) clear
	set h [llength $hdu_list]
	if { $h > 10 } {
	    set h 10
	}
	$itk_component(table) config -height $h -info $hdu_list

	# avoid endless loop due to new image commands caused by '$image_ hdu set'
	if { "$filename_" == "$old_filename" && ! [$image_ isclear] } {
	    return
	}

        $itk_component(show) config -state disabled
        $itk_component(display) config -state disabled

	# cleanup first
	catch {destroy $itk_component(bintable)}
	
	if {"$filename_" == ""} {
	    return
	}

	# See if there is more than one image, otherwise skip it.
	# If there are multiple images and they are the same size,
	# put them in "crpix" order also.

	set num_images_ 0
	catch {unset ext_}
	set max_crpix1 [set max_crpix2 0]
	set naxis1 [set naxis2 0]
        set first_image -1
	set use_crpix 1
	foreach hdu $hdu_list {
	    eval lassign [list $hdu] $headings
            if {"$CRPIX1" == ""} {
                set CRPIX1 0
            } 
            if {"$CRPIX2" == ""} {
                set CRPIX2 0
            } 
	    if {"$Type" == "image" && "$NAXIS" >= 2 && "$NAXIS1" > 0 && "$NAXIS2" > 0} {
		set ext_($num_images_,HDU) $HDU
		set ext_($num_images_,ExtName) $ExtName
		set ext_($num_images_,NAXIS1) $NAXIS1
		set ext_($num_images_,NAXIS2) $NAXIS2
		set ext_($num_images_,CRPIX1) $CRPIX1
		set ext_($num_images_,CRPIX2) $CRPIX2
		if {$first_image == -1} {
                    set first_image $HDU
		    set max_crpix1 $CRPIX1
		    set max_crpix2 $CRPIX2
		    set naxis1 $NAXIS1
		    set naxis2 $NAXIS2
		} else {
		    if {$naxis1 != $NAXIS1 || $naxis2 != $NAXIS2} {
			set use_crpix 0
		    }
		    if {$CRPIX1 > $max_crpix1} {
			set max_crpix1 $CRPIX1
		    }
		    if {$CRPIX2 > $max_crpix2} {
			set max_crpix2 $CRPIX2
		    }
		}
		incr num_images_
	    } 
	}

	if {$num_images_ < 1} {
	    return
	}

	# determine the table index for each image
	if {$use_crpix} {
	    # put images in correct order based on crpix values 
	    set max_row_ 0
	    set max_col_ 0
	    for {set i 0} {$i < $num_images_} {incr i} {
		set crpix1 $ext_($i,CRPIX1)
		set crpix2 $ext_($i,CRPIX2)
		set naxis1 $ext_($i,NAXIS1)
		set naxis2 $ext_($i,NAXIS2)
		set row [expr {round(double($max_crpix2 - $crpix2)/$naxis2)}]
		set col [expr {round(double($max_crpix1 - $crpix1)/$naxis1)}]
		if {$row<0 || $col<0 || [info exists check($row,$col)]} {
		    # put in sequential order
		    set use_crpix 0
		    break
		}
		set check($row,$col) 1
		set ext_($i,row) $row
		set ext_($i,col) $col
		set max_row_ [max $max_row_ $row]
		set max_col_ [max $max_col_ $col]
	    }
	} 

	if {! $use_crpix} {
	    # different sized images: put in sequential order
	    set num_cols 4
	    set num_rows [expr {$num_images_/$num_cols+$num_cols}]
	    set max_row_ 0
	    set max_col_ 0
	    set n 0
	    for {set row 0} {$row < $num_rows} {incr row} {
		for {set col 0} {$col < $num_cols} {incr col} {
		    if {$n == $num_images_} {
			break
		    }
		    set ext_($n,row) $row
		    set ext_($n,col) $col
		    incr n
		    set max_row_ [max $max_row_ $row]
		    set max_col_ [max $max_col_ $col]
		}
	    }
	}

	# Select the HDU being displayed, if any
        if {$first_image == -1} {
            select_image_hdu [$image_ hdu]
        } else {
            select_image_hdu $first_image
        }

        if { $num_images_ > 1 } {
            $itk_component(show) config -state normal
            if { $use_crpix } {
                $itk_component(display) config -state normal
            }
        }

	show_images
    }

    # Switch off the show button and remove all image minature versions

    protected method quit_images {} {
	global $w_.show
	set $w_.show 0
	delete_images
    }

    # Remove all image minature versions

    protected method delete_images {} {
        if {! [info exists itk_component(hduimages)]} {return}
	save_images_geometry
	catch {destroy $itk_component(hduimages)}
    }

    # Save the geometry of the HDU images window so we can reload it the next time

    protected method save_images_geometry {} {
	set svgeoimg_ [wm geometry $itk_component(hduimages)]
    }

    # Show all image minature versions (called by checkbutton)

    protected method show_images {} {
        global $w_.show
        if { [set $w_.show] } {
	    busy {
		if { ![info exists itk_component(hduimages)] || "$svinfo_" != [svinfo] } {
		    delete_images
		    create_images
		    wm protocol $itk_component(hduimages) WM_DELETE_WINDOW [code $this quit_images]
		    set svinfo_ [svinfo]
		}
		display_images
	    }
	} else {
	    delete_images
	}
    }

    # Put the images in the table

    protected method create_images {} {
	if { [info exists itk_component(hduimages)] } {return}
	catch {unset wdg_}

	set topw [winfo parent $w_]
	itk_component add hduimages {
	    util::TopLevelWidget $topw.hduimages -transient 0
	}

	set name "FITS HDU images: [file tail $filename_]"
	wm title $itk_component(hduimages) $name
	wm iconname $itk_component(hduimages) "FITS HDU images"

	# Frame (BLT table) used to display images in FITS extensions
	itk_component add imagetab {
	    frame $itk_component(hduimages).imagetab
	}

	for {set i 0} {$i < $num_images_} {incr i} {
	    set f [frame $itk_component(imagetab).f$i -borderwidth 1 -relief raised]
	    set im [RtdImage $f.im \
			-graphics 0 \
			-displaymode 0 \
			-usexshm 0 \
			-usexsync 0 \
			-fillwidth 100 \
			-fillheight 100]
	    pack $im -fill both -expand 1 -padx 1 -pady 1

	    # save widget names for later reference
	    set wdg_($i,frame) $f
	    set wdg_($i,RtdImage) $im
	    set wdg_($i,image) [$im get_image]
	}

	itk_component add butframe {
	    frame $itk_component(hduimages).butframe
	}

	itk_component add settings {
	    button $itk_component(butframe).settings \
		-text "Use Settings from Main Image" \
		-command [code $this use_settings_from_main_image]
	}
        add_short_help $itk_component(settings) \
	    {Set the cut levels and colormap for the preview images to the one used in the main image}

 	itk_component add closeimages {
	    button $itk_component(butframe).closeimages \
		-text Close \
		-command [code $this quit_images]
	}
        add_short_help $itk_component(closeimages) \
	    {Close the HDU images display window}

 	itk_component add autocut {
	    button $itk_component(butframe).autocut \
		-text Autocut \
		-command [code $this auto_set_cut_levels]
	}
        add_short_help $itk_component(autocut) \
	    {Auto set cut levels individually to 99% for each HDU image}

	pack $itk_component(settings) $itk_component(autocut) $itk_component(closeimages) \
	    -fill x -expand 1 -side left -padx 0.5m -ipadx 0.5m
	pack $itk_component(butframe) -fill x -expand 0 -side bottom
	pack $itk_component(imagetab) -fill both -expand 1 -side top
	
	# save the geometry of the HDU images window whenever it is resized
	bind $itk_component(imagetab) <Configure> [code $this save_images_geometry]

	if { "$svinfo_" == "[svinfo]" && ![lempty $svgeoimg_] } {
	    wm geometry $itk_component(hduimages) $svgeoimg_
	}

	# Uncomment the following when a transient window is required
	# tkwait visibility $itk_component(hduimages)
	# wm transient $itk_component(hduimages) $topw
    }
    
    # Return saved info about the HDUs
    
    private method svinfo {} {
	set list [array get ext_ *NAXIS*]
	loop i 0 [llength $list] 2 {
	    lappend newlist "[lindex $list $i] [lindex $list [expr {$i+1}]]"
	}
	set newlist [lsort -increasing -index 0 $newlist]
	return $newlist
    }
    
    # Display the images

    protected method display_images {} {
	lassign [$image_ cut] low high

	for {set i 0} {$i < $num_images_} {incr i} {
	    set width $ext_($i,NAXIS1)
	    set height $ext_($i,NAXIS2)

	    # '$image_ view update' is much faster than '$image_ config -file' as used by earlier versions

	    if { [catch {$image_ view update $wdg_($i,image) $width $height image} msg] } {
		error_dialog "HDU $ext_($i,HDU), Extension '$ext_($i,ExtName)':\n$msg"
		break
	    }

	    add_image_bindings $wdg_($i,RtdImage) $ext_($i,HDU) $ext_($i,ExtName)
	    $wdg_($i,image) cut $low $high
	}
	

	# position the images in the table
	catch {blt::table forget $itk_component(imagetab)}

	set hdu [$image_ hdu]
	set idx [expr {$hdu -1}]
	if { $ext_(0,HDU) == 2 } {
	    set idx [expr {$idx -1}]
	}
	set naxis1 $ext_($idx,NAXIS1)
	if { $hdu > 0 && [$image_ width] != $naxis1 } {
	    set hdu -1
	}

	for {set i 0} {$i < $num_images_} {incr i} {
	    set row [expr {$max_row_-$ext_($i,row)}]
	    set col $ext_($i,col)
	    blt::table $itk_component(imagetab) ${row},${col} $itk_component(imagetab).f$i -fill both
	    if {"$ext_($i,HDU)" == $hdu} {
		$wdg_($i,frame) configure -relief sunken -bg red
	    }
	}
    }


    # Set the cut levels and colormap for the image extensions to the ones
    # used in the main image

    method use_settings_from_main_image {} {
	# return if no image
	if {! [info exists wdg_(0,image)]} {
	    return
	}

	lassign [$image_ cut] low high
	if { "$low" == "" || "$high" == "" || [$image_ isclear] } {
	    return
	}
	set cmap [$image_ cmap file]
	set itt [$image_ itt file]
	set colorscale [$image_ colorscale]

	set setflag 0
	set settings "$cmap $itt $colorscale"
	if { "$settings" != "$settings_" } {
	    set settings_ $settings
	    set setflag 1
	}

	$itk_component(hduimages) busy {
	    for {set i 0} {$i < $num_images_} {incr i} {
		if { $setflag } {
		    $wdg_($i,image) colorscale $colorscale
		    $wdg_($i,image) cmap file $cmap
		    $wdg_($i,image) itt file $itt
		}
		$wdg_($i,image) cut $low $high
		update idletasks
	    }
	}
    }

    
    # Set the cut levels for the image extensions to the given percent

    method auto_set_cut_levels {{percent 99}} {
	$itk_component(hduimages) busy {
	    for {set i 0} {$i < $num_images_} {incr i} {
		$wdg_($i,image) autocut -percent $percent
		update idletasks
	    }
	}
    }

    
    # This method is called when the user clicks on an image HDU icon.
    # Display the selected image and disable the delete button.
    
    protected method select_image_hdu {hdu} {
	$itk_component(delete) config -state disabled

	# display only a real image
	set headings [$image_ hdu listheadings]
	eval lassign [list [lindex [$image_ hdu list] [expr {$hdu -1}] ]] $headings
	if {"$Type" == "image" && "$NAXIS" < 1} {
	    return
	}

	for {set i 0} {$i < $num_images_} {incr i} {
	    if {[info exists wdg_($i,frame)] && [winfo exists $wdg_($i,frame)]} {
		if {"$ext_($i,HDU)" == "$hdu"} {
		    $wdg_($i,frame) configure -relief sunken -bg red
		} else {
		    $wdg_($i,frame) configure -relief raised -bg [$itk_component(imagetab) cget -background]
		}
	    }
	}
	lassign [$image_ cut] low high
	busy {
	    $image_ hdu $hdu
	    update idletasks
	    $image_ cut $low $high
	}
	catch {$itk_component(table) select_row [expr {$hdu-1}]}
    }

    
    # Add bindings to the given RtdImage itcl class object and set it to 
    # display the given HDU when clicked on. 
    # The image's extension name is also given.

    protected method add_image_bindings {im hdu name} {
	set image [$im get_image]
	set canvas [$im get_canvas]

	# set the HDU for the image
	$image hdu $hdu

	# need to add 2 bindings: one for the image, one for the background
	bind $canvas <1> [code $this select_image_hdu $hdu]

	# set up a resize handler to change the image size
	bind $canvas <Configure> [code $this resize $im %w %h]

	# add a help message indicating which image it is
	set s $name
	if {"$s" != ""} {
	    set s "($s)"
	}
	add_short_help $canvas "Click here to display HDU $hdu $s"
    }


    # This method is called when the image window is resized.
    # The RtdImage widget and the new width and height are given.

    protected method resize {im new_width new_height} {
	set image [$im get_image]
	$image config -fillwidth $new_width -fillheight $new_height
	$im center
    }


    # Set the HDU to display. Makes the currently selected HDU the current HDU

    protected method set_hdu {} {
	set sel [$itk_component(table) get_selected]
	if {[llength $sel]} {
	    lassign [lindex $sel 0] hdu type name
	    if {"$type" == "image"} {
		busy {
		    select_image_hdu $hdu
		}
	    } elseif {"$type" == "ascii" || "$type" == "binary"} {
		display_fits_table $name $hdu
	    }
	}
    }
    

    # This method is called when a line in the HDU list is selected.
    # Update the states of the buttons depending on the selection.

    protected method select_hdu {} {
	set sel [$itk_component(table) get_selected]
	if {[llength $sel]} {
	    lassign [lindex $sel 0] hdu type name
	    if {"$type" == "image"} {
		$itk_component(delete) config -state disabled
	    } elseif {"$type" == "ascii" || "$type" == "binary"} {
		$itk_component(delete) config -state normal
	    }
	}
    }
    

    # Display all of the image extensions as a single image (combine based 
    # on CRPIX1 and CRPIX2 keywords).

    protected method display_as_one_image {} {
	for {set i 0} {$i < $num_images_} {incr i} {
	    if {[info exists wdg_($i,frame)] && [winfo exists $wdg_($i,frame)]} {
		$wdg_($i,frame) configure -relief raised \
		    -bg [$itk_component(imagetab) cget -background]
	    }
	}
	busy { $image_ hdu display }
    }


    # Select an HDU to display. This makes it the current HDU

    protected method delete_hdu {} {
	set hdu [$image_ hdu]
	set selected_hdu [lindex [lindex [$itk_component(table) get_selected] 0] 0]
	set type [$image_ hdu type $selected_hdu]

	if {"$type" == "image"} {
	    error_dialog "Deleting image HDUs is not allowed"
	    return
	} else {
	    append type " table"
	}
	if {! [confirm_dialog "Delete the $type at HDU $selected_hdu ?"]} {
	    return
	}

	if {"$type" == "image"} {
	    catch {destroy $imagetab.f.im$selected_hdu}
	}

	if {[catch {$image_ hdu delete $selected_hdu} msg]} {
	    error_dialog $msg
	}

	set n [$image_ hdu count]
	if {$hdu == $selected_hdu || $n <= 0 || $hdu > $n} {
	    $itk_option(-image) clear
	    return
	}

	# current HDU my have moved up...
	$image_ hdu $hdu

	# update the list
	show_hdu_list
    }


    # Display the current FITS table. Needs to be redefined by parent class
    # when other tables than binary are to be displayed.

    protected method display_fits_table {name hdu} {
        set sel [$itk_component(table) get_selected]
        lassign [lindex $sel 0] hdu type name
        if {"$type" != "binary"} { return }

        set bintbl $w_.bintable
        # toggle the visibility of the binary table
        if { [winfo exists $bintbl] && $hdu == $hdu_bin_ } {
            destroy $bintbl
            return
        }
        
        if { ! [winfo exists $bintbl] } {
            # TableList(n) widget for displaying the binary table
	    itk_component add bintable {
		util::TableList $bintbl \
		    -hscroll 1
	    }
            pack $itk_component(bintable) -fill both -expand 1
        }

        set hdu_bin_ $hdu
        set headings [$image_ hdu headings $hdu]
	busy {
	    set info [$image_ hdu get $hdu]
	}
        $itk_component(bintable) config -title $name -headings $headings -info $info
    }

    
    # -- options  --

    # target SkyCatCtrl itcl class object
    itk_option define -image image Image {} {
	set image_ [$itk_option(-image) get_image]
    }

    # flag: if true, images are "subsampled" when shrinking, 
    # otherwise the pixels are averaged
    itk_option define -subsample subsample Subsample {1}
    
    # X shared memory option
    itk_option define -usexshm useXshm UseXshm {0}

    # X synchronisation option
    itk_option define -usexsync useXsync UseXsync {0}

    # flag: if true, print diagnostic messages
    itk_option define -verbose verbose Verbose {0}

    # Default: show image extensions, bool
    itk_option define -show_images show_images Show_images {0}

    # optionally specify TopLevelWidget to display short help messages
    itk_option define -shorthelpwin shortHelpWin ShortHelpWin {}

    # -- protected vars --
    
    # internal rtdimage object
    protected variable image_

    # name of image file
    protected variable filename_ {}

    # number of image HDUs in the current FITS file
    protected variable num_images_ 0

    # array(HDUIndex,keyword) of image keyword
    protected variable ext_

    # array(HDUIndex,keyword) of widget info
    protected variable wdg_

    # max row and column
    protected variable max_row_ 0
    protected variable max_col_ 0

    # HDU number of last binary table
    protected variable hdu_bin_ -1

    # saved info for images display
    protected variable svinfo_ {}
    protected variable svgeoimg_ {}
    protected variable settings_ {}
}


