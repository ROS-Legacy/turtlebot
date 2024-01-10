FROM ros:kinetic-robot-xenial

RUN apt-get update && \
    apt-get install -y ros-kinetic-turtlebot && \
    rm -rf /var/lib/apt/lists/*

CMD ["roscore"]
