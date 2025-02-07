MAKE_DIR		= srcs

DOCKER_IMAGE    = kfs
DOCKER_PORT     = 8888

all: kernel
	docker build -t ${DOCKER_IMAGE} .
	docker run --rm -d -p ${DOCKER_PORT}:5900 ${DOCKER_IMAGE}
	sleep 0.1
	vncviewer 0.0.0.0:${DOCKER_PORT}

libk:
	${MAKE} -C ${MAKE_DIR}/libc

kernel: libk
	${MAKE} -C ${MAKE_DIR}/kernel

clean:
	${MAKE} -C ${MAKE_DIR}/libc clean
	${MAKE} -C ${MAKE_DIR}/kernel clean

fclean:
	${MAKE} -C ${MAKE_DIR}/libc fclean
	${MAKE} -C ${MAKE_DIR}/kernel fclean
	docker ps -aq --filter "ancestor=${DOCKER_IMAGE}" | xargs -r docker stop
	docker ps -aq --filter "ancestor=${DOCKER_IMAGE}" | xargs -r docker rm

re: fclean all

.PHONY: all libk kernel clean fclean re
