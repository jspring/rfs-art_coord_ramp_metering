#!/bin/bash

# Introduction

#This guide is intended to provide you with simple instructions on how to install Nagios from source (code) on Ubuntu and have it monitoring your local machine inside of 20 minutes. No advanced installation options are discussed here - just the basics that will work for 95% of users who want to get started.

#These instructions were written based on an Ubuntu 6.10 (desktop) installation. They should work for an Ubuntu 7.10 install as well.

#What You'll End Up With

#If you follow these instructions, here's what you'll end up with:

#    Nagios and the plugins will be installed underneath /usr/local/nagios
#    Nagios will be configured to monitor a few aspects of your local system (CPU load, disk usage, etc.)
#    The Nagios web interface will be accessible at http://localhost/nagios/

#Required Packages

#Make sure you've installed the following packages on your Ubuntu installation before continuing.

#    Apache 2
#    PHP
#    GCC compiler and development libraries
#    GD development libraries

#You can use apt-get to install these packages by running the following commands:

sudo apt-get install apache2 &&

sudo apt-get install libapache2-mod-php5 &&

sudo apt-get install build-essential &&

#With Ubuntu 6.10, install the gd2 library with this command:

#sudo apt-get install libgd2-dev

#With Ubuntu 7.10, the gd2 library name has changed, so you'll need to use the following:

sudo apt-get install libgd2-xpm-dev &&

#1) Create Account Information

#Become the root user.

#sudo -s &&

#Create a new nagios user account and give it a password.

sudo /usr/sbin/useradd -m -s /bin/bash nagios &&

sudo passwd nagios &&

#On older Ubuntu server editions (6.01 and earlier), you will need to also add a nagios group (it's not created by default). You should be able to skip this step on desktop, or newer server editions of Ubuntu.

#/usr/sbin/groupadd nagios

#/usr/sbin/usermod -G nagios nagios

#Create a new nagcmd group for allowing external commands to be submitted through the web interface. Add both the nagios user and the apache user to the group.

sudo /usr/sbin/groupadd nagcmd &&

sudo /usr/sbin/usermod -a -G nagcmd nagios &&

sudo /usr/sbin/usermod -a -G nagcmd www-data &&

#2) Download Nagios and the Plugins

#Create a directory for storing the downloads.

#mkdir ~/downloads &&

cd ~/downloads &&

#Download the source code tarballs of both Nagios and the Nagios plugins (visit http://www.nagios.org/download/ for links to the latest versions). These directions were tested with Nagios 3.1.1 and Nagios Plugins 1.4.11.

wget http://prdownloads.sourceforge.net/sourceforge/nagios/nagios-3.5.0.tar.gz &&

wget http://sourceforge.net/projects/nagiosplug/files/nagiosplug/1.4.16/nagios-plugins-1.4.16.tar.gz/download &&

#3) Compile and Install Nagios

#Extract the Nagios source code tarball.

cd ~/downloads &&

tar xzf nagios-3.5.0.tar.gz &&

cd nagios &&

#Run the Nagios configure script, passing the name of the group you created earlier like so:

./configure --with-command-group=nagcmd &&

#Compile the Nagios source code.

make all &&

#Install binaries, init script, sample config files and set permissions on the external command directory.

sudo make install &&

sudo make install-init &&

sudo make install-config &&

sudo make install-commandmode &&

#Don't start Nagios yet - there's still more that needs to be done...

#4) Customize Configuration

#Sample configuration files have now been installed in the /usr/local/nagios/etc directory. These sample files should work fine for getting started with Nagios. You'll need to make just one change before you proceed...

#Edit the /usr/local/nagios/etc/objects/contacts.cfg config file with your favorite editor and change the email address associated with the nagiosadmin contact definition to the address you'd like to use for receiving alerts.

sudo vi /usr/local/nagios/etc/objects/contacts.cfg &&

#5) Configure the Web Interface

#Install the Nagios web config file in the Apache conf.d directory.

sudo make install-webconf &&

#Create a nagiosadmin account for logging into the Nagios web interface. Remember the password you assign to this account - you'll need it later.

sudo htpasswd -c /usr/local/nagios/etc/htpasswd.users nagiosadmin &&

#Restart Apache to make the new settings take effect.

sudo /etc/init.d/apache2 reload &&

#Note Note: Consider implementing the ehanced CGI security measures described here to ensure that your web authentication credentials are not compromised.

#6) Compile and Install the Nagios Plugins

#Extract the Nagios plugins source code tarball.

cd ~/downloads &&

tar xzf nagios-plugins-1.4.16.tar.gz &&

cd nagios-plugins-1.4.16 &&

#Compile and install the plugins.
#But first, install SSL support to prevent HAVE_SSL error

sudo apt-get install libssl-dev &&

./configure --with-nagios-user=nagios --with-nagios-group=nagios &&

make &&

sudo make install &&

#7) Start Nagios

#Configure Nagios to automatically start when the system boots.

sudo ln -s /etc/init.d/nagios /etc/rcS.d/S99nagios &&

#Verify the sample Nagios configuration files.

sudo /usr/local/nagios/bin/nagios -v /usr/local/nagios/etc/nagios.cfg &&

#If there are no errors, start Nagios.

sudo /etc/init.d/nagios start &&

#8) Login to the Web Interface

#You should now be able to access the Nagios web interface at the URL below. You'll be prompted for the username (nagiosadmin) and password you specified earlier.

http://localhost/nagios/ &&

#Click on the "Service Detail" navbar link to see details of what's being monitored on your local machine. It will take a few minutes for Nagios to check all the services associated with your machine, as the checks are spread out over time.

#9) Other Modifications

#If you want to receive email notifications for Nagios alerts, you need to install the mailx (Postfix) package.

#sudo apt-get install mailx &&
sudo apt-get install mailutils &&

sudo apt-get install postfix &&

#You'll have to edit the Nagios email notification commands found in /usr/local/nagios/etc/objects/commands.cfg and change any '/bin/mail' references to '/usr/bin/mail'. Once you do that you'll need to restart Nagios to make the configuration changes live.

sudo /etc/init.d/nagios restart

#Configuring email notifications is outside the scope of this documentation. Refer to your system documentation, search the web, or look to the Nagios Support Portal or Nagios Community Wiki for specific instructions on configuring your Ubuntu system to send email messages to external addresses.

