# How to build a new image

You have to login to docker with `theopenscholar` account.

```
(cd docker/travis)
docker build -t theopenscholar/openscholar-env .
docker push theopenscholar/openscholar-env
```

# How to run locally the travis process

- If you want to run restful test, run `export TEST_SUITE=restful`
- Open `.travis.yml` file and follow `script` steps

# How to use image for local development (experimental)

- `cp docker/travis/docker-compose.yml .`
- `cp docker/travis/docker-compose.override.yml.local docker-compose.override.yml`
- place an SQL dump in docker/dump
- `docker-compose up -d`
- May edit default/settings.php database credential
```
 MYSQL_ROOT_PASSWORD: password
 MYSQL_DATABASE: scholar
 MYSQL_USER: scholar
 MYSQL_PASSWORD: drupal
```
- Visit http://localhost
