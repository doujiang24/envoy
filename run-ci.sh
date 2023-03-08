
set -x

docker_env="-e http_proxy=http://30.182.136.77:8123 -e https_proxy=http://30.182.136.77:8123 -e GOPROXY=https://goproxy.cn -e socks_proxy=socks://30.182.136.77:13659"

target=//contrib/golang/filters/http/test:golang_filter_fuzz_test
#target=//test/...

ENVOY_DOCKER_BUILD_DIR=~/.bazel-cache/ \
    ENVOY_DOCKER_OPTIONS="$docker_env" \
    ./ci/run_envoy_docker.sh './ci/do_ci.sh bazel.fuzz' $target
