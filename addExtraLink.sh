if [ -d "./build/CMakeFiles/LeapView.dir/" ]
then
    echo "MeshTalentSource.dir exists => concatnating Extra Flags"
    echo " "${PWD}"/Leap/Lib/libc++/libLeap.dylib" > linkExtra.txt

    linkDir="./build/CMakeFiles/LeapView.dir/"

    #linkDir="./"
    #Print links + Extra Links
    echo $linkDir"link.txt" "linkExtra.txt" ">>" $linkDir"link.txt"
    perl -i -pe 'chomp if eof' $linkDir"link.txt"

    #Save link to link1
    cat $linkDir"link.txt" "linkExtra.txt" >> $linkDir"link1.txt"

    #Copy link1 to link
    cp $linkDir"link1.txt" $linkDir"link.txt"

    #remove link1
    rm $linkDir"link1.txt"
else
    echo "Make sure to name build directory => 'build'"
fi