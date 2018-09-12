#!/usr/bin/perl

print("start (wait for network connection)\n");
print('waiting(ppp0)...');
for ($i=0; $i<100; $i++){
  $ret = `ip addr`;
  if ($ret =~ /ppp0/){
    last;
  }
  print('.');
  sleep 3;
}
print("\n");

print('waiting(ping)...');
for ($i=0; $i<100; $i++){
  $ret = `ping -c 3 -W 1 google.com`;
  if ($ret =~ / 0% packet loss/){
    last;
  }
  print('.');
  sleep 3;
}



print("finished\n");

