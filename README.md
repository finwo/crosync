# Finwo / tinyrest

Tiny http-based CRUD engine

---

## Security

This engine only does storage and completely disregards any security what-so-ever. If you want that, add a simple proxy in front it, add something along the lines of [oauth2](https://oauth.net/2/) for simplicity, and promote it as a database engine.

---

## Data format

For simplicity and speed, I've chosen to keep data in [percent encoding](https://en.wikipedia.org/wiki/Percent-encoding) and only support the characters from the 7-bit ASCII range. If you want unicode support, I suggest you look at [punycode](https://en.wikipedia.org/wiki/Punycode).

---

## Routes

The whole engine supports 3 methods & only 1 path structure

#### Methods
```
GET		Read record(s)
POST	Create or update record(s)
DELETE	Delete record(s)
```

All methods return the affected records in their original state, seperated by newlines (`\n`)

#### Path
```
/:object[/:id|:limit][?filter]
```

All methods support a filter (and a limit) in the query part of the requested URL.

#### Components
```
Required:

table   Each object needs a name or type.
        Internally seperates files & keep your queries cleaner

Optional:

id|limit  Limits your action to a single a <limit> number of records.
          The IDs are alphanumeric, just like in MongoDB
filter    url-encoded filter.

        Example (pseudocode notation is how the engine interprets it)

        foo=bar&forks%5Bgt%5D=20&money%5Beq%5D=$$$&name%5Bfirst%5D=root

        foo        = "bar"
        forks      > 20
        money      = "$$$"
        name.first = "root"

```

---

## Contributing

After checking the [Github issues](https://github.com/finwo/tinyrest/issues) and confirming that your request isn't already being worked on, feel free to spawn a new fork & send in a pull request to the `develop` branch.

The develop branch is merged periodically into the master after confirming it's stable, to make sure the master always contains a production-ready version.