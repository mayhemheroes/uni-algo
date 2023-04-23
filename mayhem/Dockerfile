FROM debian:bookworm as builder

RUN apt update && apt install -y clang build-essential

COPY . /uni-algo
WORKDIR /uni-algo/fuzz

RUN make

FROM debian:bookworm
COPY --from=builder /uni-algo/fuzz /