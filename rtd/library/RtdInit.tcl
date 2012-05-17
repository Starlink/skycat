# E.S.O. - VLT project
#
# "@(#) $Id: RtdInit.tcl,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"
#
# RtdInit.tcl
#
# script which is executed by RtdImage.C to initialize tcl
#
# who           when       what
# --------     ---------   ----------------------------------------------
# pbiereic     24/08/99    created

# What we depend on.
package require Tclutil
package require Astrotcl

if {![lcontain $auto_path $rtd_library]} {
    lappend auto_path $rtd_library
}

namespace eval rtd {namespace export *}
namespace import -force rtd::*
