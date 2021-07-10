#!/bin/bash

current_dir=`pwd | awk -F '/' '{print $NF}'`
if [ "scripts" != "$current_dir" ]
then
	echo "this script must be called from within the scripts directory"
	exit 1
fi

if [ -z $1 ] || [ -z $2 ]
then
	echo "Use ./create_controller.sh controller_name cmake-binary";
	exit 1
fi

cd ..

CMAKE=$2
which $CMAKE > /dev/null
if [ 0 != $? ]
then
	echo "invalid cmake-binary"
	exit 1
fi

name=$1

#Check if the controller already exists.
if [ -f include/controller/$name.h ] || [ -f src/controller/$name.cpp ]
then
	echo "Controller files with that name already exist. Aborting";
	exit 1
fi 

#Copy files and replace their contents
cp scripts/templates/_template.h include/controller/$name.h
cp scripts/templates/_template.cpp src/controller/$name.cpp
sed -i "s/_template/$name/g" include/controller/$name.h
sed -i "s/_template/$name/g" src/controller/$name.cpp

#Add to controller to the state list
sed -i "/\[new-controller-state-mark\]/i state_$name," include/controller/states.h

#Add the include and instance to the header file
sed -i "/\[new-controller-header-mark\]/i #include \"../../include/controller/$name.h\"" include/dfwimpl/state_driver.h
sed -i "/\[new-controller-property-mark\]/i \\\tptr_controller\t\t\t\t\tc_$name;" include/dfwimpl/state_driver.h

#Instance the controller in the implementation file
sed -i "/\[new-controller-mark\]/i \\\treg(c_$name, controller::t_states::state_$name, new controller::$name(log));" src/dfwimpl/state_driver.cpp

#Add the file to the cmake recipes...
sed -i "/\[new-controller-source-mark\]/i \\\t\$\{CMAKE_CURRENT_SOURCE_DIR\}/$name.cpp" src/controller/CMakeLists.txt

#Finally, if that was the first controller, just set it up...
grep -q 'state_driver_interface(controller::t_states::state_min)' src/dfwimpl/state_driver.cpp
if [ 0 -eq $? ]
then
	sed -i "s/state_driver_interface(controller::t_states::state_min)/state_driver_interface(controller::t_states::state_$name)/g" src/dfwimpl/state_driver.cpp
	echo "$name was setup as the starting controller"
fi

cd build
$CMAKE .. > /dev/null
if [ 0 != $? ]
then
	echo "cmake failed"
	exit 1
fi

echo "Done"
exit 0
