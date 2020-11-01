# dsound
dsound - a dll loader for age2_x1

usage is simple: put it in <gamefolder>\age2_x1 (or rather, next to age2_x1.exe).

create a `dsound.cfg` in the same folder, with a list of dlls to be loaded. the paths need to be relative to <gamefolder>, not <gamefolder>\age2_x1.

add a `#` or `;` in front of the line to comment the dll out.

should theoretically work for anything that uses dsound.dll, but has a hardcoded fix for age2_x1's age2_x1 folder.
