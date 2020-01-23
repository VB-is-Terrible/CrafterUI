BUILD_LOCATION=""

if [$BUILD_LOCATION = ""]; then
    mkdir "build"
else 
    mkdir $BUILD_LOCATION
    ln -s $BUILD_LOCATION "build"
fi

ln -s "build/compile_commands.json"
