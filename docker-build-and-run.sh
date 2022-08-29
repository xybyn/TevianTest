sudo docker build -t docker-cpp-sample .
sudo docker run --rm -ti --net=host -e DISPLAY=:0 docker-cpp-sample
