# A Really Basic Kernel

## Instructions

```
$ docker build -t hello-kernel-build .
$ docker run -ti -v $(pwd):/data hello-kernel-build
$ cd /data
$ make run
```
