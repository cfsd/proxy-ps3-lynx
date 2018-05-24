name="proxy-ps3-lynx-v0.0.1"
tag=$(git log -1 --pretty=%h)
img="$name:$tag"
echo $img
docker build -t $img -f Dockerfile.amd64 .


docker build -t "proxy-ps3-lynx-v0.0.1:$(git log -1 --pretty=%h)" -f Dockerfile.amd64 .
