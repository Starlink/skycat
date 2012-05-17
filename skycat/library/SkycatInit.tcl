# E.S.O. - VLT project
#
# "@(#) $Id: SkycatInit.tcl,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"
#
# SkycatInit.tcl
#
# script which is executed by Skycat.C to initialize the package
#
# who           when       what
# --------     ---------   ----------------------------------------------
# abrighto     02/01/06    created

package require img::xpm
package require Tclutil
package require Astrotcl
package require Cat

if {![lcontain $auto_path $skycat_library]} {
    lappend auto_path $skycat_library
}

namespace eval skycat {namespace export *}
namespace import -force skycat::*
