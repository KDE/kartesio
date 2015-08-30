#This simple script tells you which libraries you need to install Kartesio
ldlist=$(ldd ../build-kartesio-Desktop_Qt_in_PATH-Release/kartesio | awk -F"=> " {'print $2'} | awk -F"(" {'print $1'})
while read -r line
do 
  apt-file search $line | grep -v "$line." | awk -F": " {'print $1'}
done <<<"$ldlist"
