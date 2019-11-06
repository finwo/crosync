# finwo / crosync

Tiny http-based CRUD engine

---

## Security

This engine only does storage and completely disregards any security what-so-ever. If you want that, add a simple proxy in front it, add something along the lines of [oauth2](https://oauth.net/2/) for simplicity, and promote it as a database engine.

---

## Routes

```
GET    /       List keys
GET    /:id    Read record
POST   /:id    Create or update a record
DELETE /:id    Delete record
```
