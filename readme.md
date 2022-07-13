### quit | To exit the program.

### cd [path] | To navigate directory.

example1: cd dir1 | to enter folder named dir1

example2: cd dir2/dir3 | to enter folder named dir2 and then dir3

example3: cd .. | to enter last dir

example4: cd ../.. | to enter last dir and the one before that

### ls | To list all folders in the current path

### create [config] | To create a configuration

example1: create test1

### delete [config] | To delete a configuration

example1: delete test1

note: config must exist

### add [config] [setting=value] [setting=value]... | To add a setting to a configuration

example1: add default Gamma=12 Brightness=34

note: current dir must have the setting

### remove [config] [setting] [setting]... | To remove a setting from a configuration

example1: remove default Gamma

note: current dir must have the setting

### apply [config] | To make a configuration active

example1: apply default

### print [config] | To list the settings in a configuration

example1: print default