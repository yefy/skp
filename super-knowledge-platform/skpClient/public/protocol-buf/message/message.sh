#!/bin/bash
path_cc="    ../../../../public/protocol-buf/"
path_proto="    ../../../../public/protocol-buf/message/"
pri="../message.pri"
proto_pri="./message-proto.pri"
source_dirs="./"
source_dirs=$(echo $source_dirs|tr " " "\n")
messages=""
number=0
for source_dir in $source_dirs
do
        if [ "$source_dir" != "" ]
        then
                for line in $(ls $source_dir)
                do
                        case "$line" in
                        *.proto)
			echo -e $line
			protoc -I=./ --cpp_out=../ ./$line
			message=$(echo $line | awk -F"." '{print $1}')
			messages=$messages" "$message
			echo $messages
			number=`expr $number + 1`
			echo $number
                        ;;
                        esac
                done
        else
                break
        fi
done

echo -e "MESSAGE_CURR_PATH = \$\${PWD}" > $pri
echo -e "INCLUDEPATH +=  \\" >> $pri
echo -e "            \$\${MESSAGE_CURR_PATH}" >> $pri
echo -e "DEPENDPATH  +=  \\" >> $pri
echo -e "            \$\${MESSAGE_CURR_PATH}" >> $pri

echo -e "HEADERS += \\" >> $pri
writeNumber=0
for message in $messages
do
        if [ "$message" != "" ]
        then
		writeNumber=`expr $writeNumber + 1`
		if [ $number -eq $writeNumber ]
		then
			echo -e $path_cc$message".pb.h" >> $pri	
		else
			echo -e $path_cc$message".pb.h \\" >> $pri
		fi
        else
                break
        fi
done


echo -e "SOURCES += \\" >> $pri
writeNumber=0
for message in $messages
do
        if [ "$message" != "" ]
        then
		writeNumber=`expr $writeNumber + 1`
		if [ $number -eq $writeNumber ]
                then
			echo -e $path_cc$message".pb.cc" >> $pri
		else
			echo -e $path_cc$message".pb.cc \\" >> $pri
		fi
        else
                break
        fi
done



echo -e "MESSAGE_PROTO_CURR_PATH = \$\${PWD}" > $proto_pri
echo -e "INCLUDEPATH +=  \\" >> $proto_pri
echo -e "            \$\${MESSAGE_PROTO_CURR_PATH}" >> $proto_pri
echo -e "DEPENDPATH  +=  \\" >> $proto_pri
echo -e "            \$\${MESSAGE_PROTO_CURR_PATH}" >> $proto_pri

echo -e "OTHER_FILES += \\" >> $proto_pri
writeNumber=0
for source_dir in $source_dirs
do
        if [ "$source_dir" != "" ]
        then
                for line in $(ls $source_dir)
                do
                        case "$line" in
                        *.proto)
			writeNumber=`expr $writeNumber + 1`
			if [ $number -eq $writeNumber ]
                	then
				echo -e $path_proto$line >> $proto_pri
			else
				echo -e $path_proto$line" \\" >> $proto_pri
			fi
                        ;;
                        esac
                done
        else
                break
        fi
done

