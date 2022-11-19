sudo apt-get update
sudo apt-get -y install cups
sudo apt-get install cups cups-client

<!-- otherwise use sudo
sudo adduser $USER lpadmin -->

sudo service cups restart
ppdc ev3.drv
sudo lpadmin -p EV3 -E -P ./ppd/ev3Printer2022.ppd

check to see if there
sudo lpstat -s
<!-- share printer -->
sudo lpadmin -p EV3 -o printer-is-shared=true