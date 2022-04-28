/**
 * Provides a generic module for interacting with entities via our REST API
 */
(function () {

  var restPath = '',
    entities = {},
    fetched = {},
    defers = {},
    cache = {},
    weSaved = {},
    lockPromise;

  angular.module('EntityService', ['indexedDB'])
    .config(function () {
      restPath = Drupal.settings.paths.api;
    })
  /**
   * Provider to manage configuration options for EntityService
   */
    .provider('EntityConfig', [function () {
      var config = {};

      function initType(type) {
        config[type] = config[type] || {
          fields: {}
        };
      }

      return {
        $get: [function () {
          return angular.copy(config);
        }],
        addField: function (type, field, value) {
          initType(type);

          config[type].fields[field] = value;
        }
      }
    }])
  /**
   * Service to maintain the list of files on a user's site
   */
    .factory('EntityService', ['$rootScope', '$http', '$q', 'EntityConfig', '$indexedDB', 'EntityCacheUpdater', function ($rootScope, $http, $q, config, $idb, ECU) {
      var factory = function (entityType, idProp) {
        var type = entityType;
        var ents;
        entities[entityType] = ents = entities[entityType] || {};
        var entityCount = 0;
        var eventName = 'EntityService.' + type;
        var errorAttempts = 0;
        var vsite = null;
        var fetchDefer;

        if (Drupal.settings.spaces) {
          vsite = Drupal.settings.spaces.id;
        }

        var success = function(resp, status, headers, config) {
          var key = config.pKey;
          recursiveFetch(resp, status, headers, config);
        }

        function recursiveFetch(resp, status, headers, config) {
          var key = config.pKey;
          // convert the key into a params array
          for (var i=0; i<resp.data.length; i++) {
            cache[key].data.push(resp.data[i]);
          }

          if (resp.next) {
            var max = Math.ceil(resp.count/resp.data.length),
              curr = resp.next.href.match(/page=([\d]+)/)[1];
            defers[key].notify(("Loading $p% complete.").replace('$p', Math.round(((curr-1)/max)*100)));
            $http.get(resp.next.href, {pKey: key}).success(recursiveFetch);
          }
          else {
            saveCache(key);
            defers[key].resolve(angular.copy(cache[key].data));
            $rootScope.$broadcast(eventName+'.fetch', angular.copy(cache[key].data), key);
          }
        }

        var errorFunc = function(resp, status, headers, config) {
          errorAttempts++;
          if (errorAttempts < 3) {
            $http.get(restPath + '/' + entityType, config).
              success(success).
              error(errorFunc);
          }
          else {
            defers[config.pKey].reject('Error getting files. Aborting after 3 attempts.');
          }
        };

        function findByProp(prop, value) {
          for (var i in ents) {
            if (ents[i][prop] && ents[i][prop] == value) {
              return i;
            }
          }
        }

        this.fetchOne = function (id) {
          var cKey = entityType + ':' + id;

          if (!defers[cKey]) {
            var url = restPath + '/' + entityType + '/' + id;
            defers[cKey] = $q.defer();
            $http.get(url, {pKey: cKey})
              .then(function (response) {
                ents[id] = response.data.data[0];
                defers[cKey].resolve(angular.copy(response.data.data[0]));
              },
              function (response) {
                defers[cKey].reject(response);
              });
          }
          return defers[cKey].promise;
        };

        this.fetchImageStyle = function(id, imageStyle) {
          var url = restPath + '/' + entityType + '/' + id + '/image_style/' + imageStyle;
          var cKey = entityType + ':' + id;

          return $http.get(url, {pKey: cKey});
        };

        this.fetch = function (params) {
          if (!params) {
            params = {};
          }

          if (vsite) {
            params.vsite = vsite;
          }

          if (config[entityType]) {
            for (var k in config[entityType].fields) {
              params[k] = config[entityType].fields[k];
            }
          }

          var key = entityType + ':' + JSON.stringify(params);
          if (!defers[key]) {
            defers[key] = $q.defer();

            lockPromise.then(function (keys) {
              if (keys.indexOf(key) === -1) {
                var url = restPath + '/' + entityType;
                $http.get(url, {params: params, pKey: key})
                  .success(success)
                  .error(errorFunc);
                setTimeout(function () {
                  defers[key].notify("Loading 0% complete.");
                }, 1);
                cache[key] = {
                  key: key,
                  lastUpdated: parseInt(Date.now()/1000),
                  data: [],
                  entityType: entityType,
                  idProperty: idProp,
                  params: params,
                  matches: function(entity, entityType) {
                    if (entityType != this.entityType) {
                      return false;
                    }
                    return testEntity.call(this, entity);
                  }
                }
              }

              return keys;
            });

            defers[key].promise.then(function(data) {
              for (var i = 0; i < data.length; i++) {
                ents[data[i][idProp]] = data[i];
              }

              return data;
            });
          }
          return defers[key].promise;
        }

        this.get = function (id) {
          var keys = getCacheKeysForId(entityType, idProp, id);
          for (var k in keys) {
            return cache[k].data[keys[k]];
          }
        };

        this.getCount = function () {
          return entityCount;
        };

        this.add = function (entity) {
          var k = findByProp(idProp, entity[idProp]);
          if (entities[k]) {
            throw new Exception('Cannot add entity of type ' + type + ' that already exists.');
          }

          if (vsite) {
            entity.vsite = vsite;
          }

          if (config[entityType]) {
            for (var k in config[entityType].fields) {
              params[k] = config[entityType].fields[k];
            }
          }

          // rest API call to add entity to server
          return $http.post(restPath + '/' + entityType, entity)
            .success(function (resp) {
              var entity = resp.data[0];

              weSaved[entity[idProp]] = entity.changed;
              addToCaches(entityType, idProp, entity);

              if (typeof ents[entity[idProp]] == 'undefined') {
                $rootScope.$broadcast(eventName + '.add', entity);
              }
              // this file already existed on the server
              else {
                $rootScope.$broadcast(eventName + '.update', entity);
              }
              ents[entity[idProp]] = entity;
            })
        };

        this.edit = function (entity, ignore) {
          if (!entity[idProp]) {
            this.add(entity);
            return;
          }
          ignore = ignore || [];
          ignore.push(idProp);

          var keys = getCacheKeysForEntity(type, idProp, entity),
            data = {};
          for (var k in keys) {
            data = getDiff(cache[k].data[keys[k]], entity, ignore);
            break;
          }
          var url = [restPath, entityType, entity[idProp]];

          if (data.length) {
            delete data.length;

            var config = {
              headers: {}
            };

            var updated = 0;
            if (weSaved[entity[idProp]] == undefined) {

              for (var k in keys) {
                updated = Math.max(updated, cache[k].lastUpdated);
              }
            }
            else {
              updated = weSaved[entity[idProp]];
            }
            config.headers['If-Unmodified-Since'] = (new Date(updated*1000)).toString().replace(/ \([^)]*\)/, '');

            return $http.patch(url.join('/'), data, config)
              .success(function (resp) {
                var entity = resp.data[0],
                  k = findByProp(idProp, entity[idProp]);
                ents[k] = entity;

                weSaved[entity[idProp]] = entity.changed;
                $rootScope.$broadcast(eventName + '.update', angular.copy(entity));
                var keys = getCacheKeysForEntity(type, idProp, entity);
                for (var k in keys) {
                  cache[k].data[keys[k]] = entity;
                  saveCache(k);
                }
              })
              .then(angular.noOp, function (resp) {
                switch (resp.status) {
                  case 409:
                    console.log('conflict');
                    ECU.update(entityType);
                    break;
                  case 410:
                    console.log('resource gone');
                    ECU.update(entityType);
                }
                return $q.reject(resp);
              });
          }
          else {
            var defer = $q.defer();
            defer.resolve({detail: "No data sent with request."});
            return defer.promise;
          }
        };

        this.delete = function (entity) {

          var config = {
            headers: {}
          };

          var keys = getCacheKeysForEntity(type, idProp, entity);
          var updated = 0;
          if (weSaved[entity[idProp]] == undefined) {

            for (var k in keys) {
              updated = Math.max(updated, cache[k].lastUpdated);
            }
          }
          else {
            updated = weSaved[entity[idProp]];
          }
          config.headers['If-Unmodified-Since'] = (new Date(updated*1000)).toString().replace(/ \([^)]*\)/, '');

          //rest API call to delete entity from server
          return $http.delete(restPath+'/'+entityType+'/'+entity[idProp], config).success(function (resp) {
            var k = findByProp(idProp, entity[idProp]);
            delete ents[k];

            $rootScope.$broadcast(eventName+'.delete', entity[idProp]);
            var keys = getCacheKeysForEntity(type, idProp, entity);
            for (var k in keys) {
              cache[k].data.splice(keys[k], 1);
              saveCache(k);
            }
          });
        }

        // registers an entity with this service
        // used for entities that are added outside of this service
        this.register = function (entity) {
          entity = angular.copy(entity);
          ents[entity[idProp]] = entity;

          weSaved[entity[idProp]] = entity.changed;
          addToCaches(entityType, idProp, entity);
        };
      }

      function getDiff(oEntity, nEntity, ignore) {
        var diff = {},
          numProps = 0;

        // Skip the property "$$hashKey" that's been generated by angular core
        // code - It's not relevant data thus should be skipped.
        ignore.push('$$hashKey');

        for (var k in oEntity) {
          if (ignore.indexOf(k) == -1 && !compareProps(oEntity[k], nEntity[k])) {
            diff[k] = nEntity[k];
            numProps++;
          }
        }
        diff.length = numProps;

        return diff;
      }

      function saveCache(key) {
        $idb.openStore('entities', function (store) {
          cache[key].matches = cache[key].matches.toString();
          store.upsert(cache[key]);
          cache[key].matches = eval('(' + cache[key].matches + ')');
        });
      }

      /**
       * Add an entity to all caches it matches
       */
      function addToCaches(type, idProp, entity) {
        var keys = getCacheKeysForEntity(type, idProp, entity);

        for (var k in cache) {
          if (keys[k] != undefined) {
            cache[k].data[keys[k]] = entity;
            continue;
          }

          if (cache[k].matches(entity, type)) {
            cache[k].data.push(entity);
            saveCache(k);
          }
        }
      }

      return factory;
    }])
  .run(['EntityCacheUpdater', '$q', '$indexedDB', function (ECU, $q, $idb) {
      var entityTypes = {},
        lock = $q.defer();
      lockPromise = lock.promise;

      $idb.openStore('entities', function (store) {
        var results;
        // if we're in a vsite, only get a subset of the entityservice
        // we really don't need to loop through everything
        if (Drupal.settings.spaces && Drupal.settings.spaces.id) {
          var query = store.query()
              .$index('vsite')
              .$eq(Drupal.settings.spaces.id);

          results = store.eachWhere(query);
        }
        else {
          results = store.getAll();
        }
        results.then (function (caches) {
          var keys = {};
          for (var i = 0; i < caches.length; i++) {
            var key = caches[i].key,
              type = caches[i].entityType;
            keys[key] = key;
            cache[key] = caches[i];
            cache[key].matches = eval('(' + cache[key].matches + ')');
            if (!defers[key]) {
              defers[key] = $q.defer();
            }
            entityTypes[type] = true;
          }

          var promises = [];
          for (var t in entityTypes) {
            promises = promises.concat(ECU.update(t));
          }

          $q.all(promises).then(function (args) {
            var keys = args;
            while (Array.isArray(keys[0])) {
              var t = [];
              keys = keys.concat.apply(t, keys);
            }
            console.log(keys);

            lock.resolve(keys);
          });

        }, function (error) {
          console.log(error);
          lock.resolve([]);
          return error;
        });
      }).then(angular.noOp, function (results) {  // openStore returns a promise. We can call .then() on it to attach handlers
        console.log(results);
        if (results.indexOf('Operation pending') != -1) {
          alert('There is a problem accessing your site\'s data. Please restart your browser.');
        }
        else {
          indexedDB.deleteDatabase("EntityService");
        }
        lock.resolve([]);
        return results;
      });

      window.EntityServiceDebug = {
        setTimestamps: function (newTimestamp) {
          $idb.openStore('entities', function (store) {
            store.getAll().then(function (caches) {
              var counter = 0;
              for (var i = 0; i < caches.length; i++) {
                caches[i].lastUpdated = newTimestamp;
                store.upsert(caches[i]).then(function (e) {
                  console.log((++counter) + " of " + caches.length + " caches completed.");
                });
              }
            });
          });
        },
        delete: function () {
          var r = indexedDB.deleteDatabase("EntityService");
          r.onerror = r.onsuccess = function (e) {
            console.log(e);
          }
        }
      };
    }])
    .config(['$indexedDBProvider', function ($idbp) {
      $idbp
        .connection('EntityService')
        .upgradeDatabase(2, function (event, db, tx) {
           var store = db.createObjectStore('entities', { keyPath: "key" });
           store.createIndex('key_idx', 'key', {unique: true});
        })
        .upgradeDatabase(3, function (event, db, tx) {
            var store = tx.objectStore('entities');
            store.createIndex('vsite', 'vsite', { unique: false });
        })
        .upgradeDatabase(4, function (event, db, tx) {
          var store = tx.objectStore('entities');
          store.deleteIndex('vsite');
          store.createIndex('vsite', 'params.vsite', { unique: false });
        });
    }])
  .service('EntityCacheUpdater', ['$http', '$q', '$indexedDB', '$rootScope', 'EntityConfig', function ($http, $q, $idb, $rs, EntityConfig) {
      var urlBase = restPath + '/:type/updates/:time';

      function update(updateType) {
        var keys = {},
          timestamps = {};
        for (var key in cache) {
          var type = cache[key].entityType;
          if (updateType == undefined) {
            keys[type] = keys[type] || [];
            keys[type].push(key);
          }
          else if (updateType == type) {
            keys[type] = keys[type] || [];
            keys[type].push(key);
          }
          timestamps[type] = timestamps[type] || cache[key].lastUpdated;
        }
        var promises = [];
        for (var t in keys) {
          promises.push(fetchUpdates(t, keys[t], timestamps[t]));
        }
        return $q.all(promises);
      }

      function fetchUpdates(type, keys, timestamp, nextUrl) {
        if (Drupal.settings.spaces == undefined) {
          // We are not in a vsite context so we need to return early.
          return;
        }

        var url;
        if (nextUrl == undefined) {
          url = urlBase.replace(':type', type).replace(':time', timestamp)
        }
        else {
          url = nextUrl;
        }

        if (Drupal.settings.spaces && Drupal.settings.spaces.id) {
          if (url.indexOf('?') == -1) {
            url += '?vsite=' + Drupal.settings.spaces.id;
          }
          else {
            url += '&vsite=' + Drupal.settings.spaces.id;
          }
        }

        if (EntityConfig[type]) {
          if (type == 'files' && EntityConfig[type].fields.private == 'only') {
            if (url.indexOf('?') == -1) {
              url += '?private=' + EntityConfig[type].fields.private;
            }
            else {
              url += '&private=' + EntityConfig[type].fields.private;
            }
          }
        }

        var defer = $q.defer();

        $http.get(url).then(function (resp) {
          var isUpdates = typeof resp.data.updatesAsOf != 'undefined';
          for (var i = 0; i < keys.length; i++) {
            if (nextUrl == undefined) {
              if (resp.data.allEntitiesAsOf) {
                cache[keys[i]].data = [];
                cache[keys[i]].lastUpdated = resp.data.allEntitiesAsOf;
              }
              else if (resp.data.updatesAsOf) {
                cache[keys[i]].lastUpdated = resp.data.updatesAsOf;
              }
            }
          }

          for (var i = 0; i < resp.data.data.length; i++) {
            // get all caches this entity exists in
            var cacheKeys = getCacheKeysForEntity(type, cache[keys[0]].idProperty, resp.data.data[i])
            // handle this entity for all caches it exists in
            for (var k in cacheKeys) {
              if (resp.data.data[i].status == 'deleted') {
                cache[k].data.splice(cacheKeys[k], 1);
              }
              else {
                cache[k].data.splice(cacheKeys[k], 1, resp.data.data[i]);
              }
            }
            if (resp.data.data[i].status != 'deleted') {
              // add new entities to the caches
              for (var k in cache) {
                if (cacheKeys[k] == undefined && cache[k].matches(resp.data.data[i], type)) {
                  cache[k].data.push(resp.data.data[i]);
                }
              }
            }
          }

          if (resp.data.next) {
            var next = resp.data.next.href,
              max = Math.ceil(resp.count/resp.data.data.length),
              curr = next.match(/page=([\d]+)/)[1];
            for (var i = 0; i < keys.length; i++) {
              var k = keys[i];
              defers[k].notify(("Loading updates: $p% complete.").replace('$p', Math.round(((curr - 1) / max) * 100)));
            }
            fetchUpdates(type, keys, timestamp, next).then(function (keys) {
              defer.resolve(keys);
            });
          }
          else {
            // construct 'everything' key
            var k = {};
            if (Drupal.settings.spaces.id) {
              k.vsite = Drupal.settings.spaces.id;
            }
            for (var p in EntityConfig[type]) {
              k[p] = EntityConfig[type][p];
            }

            k = type + ":" + JSON.stringify(k);
            var serverCount = resp.data.totalEntities || resp.data.count;
            // check the count against what the server reported. If it's wrong, we need to fetch everything from scratch
            if (cache[k] && serverCount != cache[k].data.length) {
              // wipe db
              for (var i = 0; i < keys.length; i++) {
                var key = keys[i];
                cache[key].data = [];
              }
              defer.resolve([]);
            }
            else {
              defer.resolve(keys);

              // we're good, so let's update the cache with what we had.
              for (var i = 0; i < keys.length; i++) {
                var key = keys[i];
                defers[key].resolve(angular.copy(cache[key].data));
                $rs.$broadcast('EntityCacheUpdater.cacheUpdated');
                $idb.openStore('entities', function (store) {
                  cache[key].matches = cache[key].matches.toString();
                  store.upsert(cache[key]);
                  cache[key].matches = eval('(' + cache[key].matches + ')');
                });
              }
            }
          }
        }, function (response) {
          // there was an error. Probably an access thing
          for (var i = 0; i < keys.length; i++) {
            defers[keys[i]].resolve(angular.copy(cache[keys[i]].data));
          }
        });

        return defer.promise;
      }

      this.update = update;
    }]);

  /**
   * Test entity fetched with a set of params to see if it matches a cache
   *
   * Proper usage:
   *  this function should use the cache object as it's 'this', by using the call() method.
   *  Ex. testEntity.call(this, entity, params);
   */
  function testEntity(entity) {
    // params is the search query we return
    var params = this.params,
      type = this.entityType;

    // if this cache isn't for private files, and the entity is private, drop it.
    //if (!params.private && entity.schema == 'private') {
    //  return false;
    //}

    for (var k in params) {
      if ((k == 'entity_type' || k == 'bundle') && typeof entity.bundles == 'object') {
        // this thing refers to other entities, like vocabs do. It can accept multiple types of entity type / file combinations

        if (entity.bundles[params.entity_type]) {
          var found = false;
          for (var l in entity.bundles[params.entity_type]) {
            if ( entity.bundles[params.entity_type][l] == params.bundle) {
              found = true;
              break;
            }
          }
          if (!found) {
            return false;
          }
        }
        else {
          // this does not accept the entity type we are looking for
          return false;
        }
      }
      // allow for a property on the entity to be a container that we only need to match one of to pass
      else if (entity[k] instanceof Object && (typeof params[k] == 'string' || typeof params[k] == 'number')) {
        var found = false;
        for (var l in entity[k]) {
          if (entity[k][l] == params[k]) {
            found = true;
            break;
          }
        }
        if (!found) {
          return false;
        }
      }
      else if (entity[k] == undefined) {
        // this is not something that comes returned on the object
        // try other things
        switch (k) {
          case 'vsite':
            if (params[k] != Drupal.settings.spaces.id) {
              return false;
            }
            break;
          case 'private': {
            if (params[k] == 'only' && entity.schema != 'private') {
              return false;
            }
          }
        }
      }
      else if (entity[k] != params[k]) {
        return false;
      }
    }
    return true;
  }

  /**
   * Collect the keys an entity exists in for every cache available
   * TODO: Generate comparator functions on cache creation to test whether entities fit in cache or not
   *
   * @param type - entity type we're searching for
   * @param entity - The entity we're looking for in the caches
   */
  function getCacheKeysForEntity(type, idProp, entity) {
    var keys = {};
    for (var k in cache) {
      // Wrong type.
      if (k.indexOf(type) == -1) {
        continue;
      }

      // TODO: Replace this for loop with comparator function invocation. Should be much faster once those work.
      for (var i = 0; i < cache[k].data.length; i++) {
        if (cache[k].data[i][idProp] == entity[idProp]) {
          keys[k] = i;
          break;
        }
      }
    }
    return keys;
  }

  /**
   * Collect the keys an id exists in for every cache available
   */
  function getCacheKeysForId(type, idProp, id) {
    var keys = {};
    for (var k in cache) {
      // Wrong type.
      if (k.indexOf(type) == -1) {
        continue;
      }

      for (var i = 0; i < cache[k].data.length; i++) {
        if (cache[k].data[i][idProp] == id) {
          keys[k] = i;
          break;
        }
      }
    }
    return keys;
  }

  /*
   * Buncha helper functions for getting comparisons
   */

  /**
   * Compares two properties by value.
   * If property is an array or object, recurses into them.
   * @param prop1
   * @param prop2
   * @returns {boolean}
   */
  function compareProps(prop1, prop2) {
    if (typeof prop1 == 'object') {
      if (prop1 instanceof Array && prop2 instanceof Array) {
        return arrayEquals(prop1, prop2);
      }
      else if (prop1 == null && prop2 == null) {
        return true;
      }
      // rest apis return 'null' instead of an empty array
      // some widgets convert that into an empty array, and thus we need to check here
      else if (prop1 == null && prop2 instanceof Array && prop2.length == 0) {
        return true;
      }
      else if (prop2 == null && prop1 instanceof Array && prop1.length == 0) {
        return true;
      }
      // if neither property is an array, return false as usual
      else if (prop1 == null || prop2 == null) {
        return false;
      }
      else {
        return objectEquals(prop1, prop2);
      }
    }
    else if (typeof(prop1) == typeof(prop2)) {
      return prop1 == prop2;
    }
    // if one property is undefined, its usually a meta property the client uses
    // it shouldn't be used at all
    else if (typeof(prop1) == 'undefined' || typeof(prop2) == 'undefined') {
      return true;
    }
    else {
      return prop1.toString() == prop2.toString();
    }
  }

  /**
   * Recursively compares 2 arrays by value
   * @param arr1
   * @param arr2
   * @returns {boolean}
   */
  function arrayEquals(arr1, arr2) {
    if (arr1.length != arr2.length) {
      return false;
    }
    else {
      var diff = false;
      for (var i = 0; i < arr1.length; i++) {
        diff = diff || compareProps(arr1[i], arr2[i]);
      }

      return diff;
    }
  }

  /**
   * Recursively compares 2 objects by value
   * @param obj1
   * @param obj2
   * @returns {boolean}
   */
  function objectEquals(obj1, obj2) {
    var keys1 = objectKeys(obj1),
      keys2 = objectKeys(obj2),
      diff = false;

    if (arrayEquals(keys1, keys2)) {
      for (var k in obj1) {
        diff = diff || compareProps(obj1[k], obj2[k]);
      }

      return diff;
    }
    // objects have different keys
    return false;
  }

  /**
   * Returns an array of all keys on the object
   * @param obj
   * @returns {Array}
   */
  function objectKeys(obj) {
    var keys = [];
    for (var k in obj) {
      keys.push(k);
    }
    return keys;
  }
})();
