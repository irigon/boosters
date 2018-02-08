# boosters

### Installation

#### Host machine

* Install the needed tools:

```sh
sudo apt-get install build-essential linux-headers-$(uname -r) make vim
```

* Install docker

```sh
$ curl -sSL https://get.docker.com | sh
```

* Add your user to docker group

```sh
$ sudo usermod -aG docker $(whoami)
```



* Start a simple udp server on port 5000

```sh
DOCKER_LOCAL_IP=172.17.0.2
$ nc -u $DOCKER_LOCAL_IP -l 5000
```
#### Docker Container

* Start container

```sh
$ docker run --name test --rm -it alpine /bin/sh
```

* Listen over the UDP port 5000 for new frames (-p since alpine has the linux version of netcat)

```sh
$ nc -u -l -p 5000
```

#### Testing

In order to test, open a pane/terminal with each of the following:

* syslog

```sh
$ tail -f /var/log/syslog
```

* UDP client

```sh
$ while true ; do echo "xxxxxx"; sleep 1; done  | nc -u 172.17.0.2 5000
```

* Compile and test module

```sh
$ cd 03_log_on_udp/
$ make
$ sudo insmod chksum.ko
$ sudo rmmod chksum
```

* verify the results in wireshark
