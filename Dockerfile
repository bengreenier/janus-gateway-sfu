FROM docker.pkg.github.com/bengreenier/janus-gateway/builder:latest as base
RUN yum install -y make

FROM base as builder
RUN mkdir -p /tmp/build
WORKDIR /tmp/build
COPY . .
RUN mkdir -p out
WORKDIR /tmp/build/out
RUN cmake ..
RUN make

FROM docker.pkg.github.com/bengreenier/janus-gateway/janus-gateway:latest as runner
COPY --from=builder /tmp/build/out/*.so /opt/janus/lib/janus/plugins/
