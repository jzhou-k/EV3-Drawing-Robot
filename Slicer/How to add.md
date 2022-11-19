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

<!-- make fillter executable and move to root folder as well as change ownership -->
copy to /usr/lib/cups/filter/EV3Filter

chmod +x makeEV3File.py
sudo chown root:root makeEV3File.py

<!-- folder out permisions -->
make sure the filder out permissions is set so all can read and write