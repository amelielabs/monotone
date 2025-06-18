### Monotone Changelog

## 1.2.0 (2025-06-18)

This release introduces improved WAL configuration options and related functionality as well as
some important fixes.

**Features**

* Added option `wal_sync_on_create`
* Added option `wal_sync_on_close`
* Renamed `wal_rotate_wm` to `wal_size`
* `wal_crc` enabled by default
* Added `wal_recovery` and ability to recover corrupted WAL (https://github.com/amelielabs/monotone/commit/ecd7576d8496e04b942548a463d351486abb5db8)
* Added `wal_sync_interval` and background WAL fsync worker (https://github.com/amelielabs/monotone/commit/559ddc8419696b5bcc5760f4be6f816b916cc979)

**Fixes**

* Fixed partition double wal write during recovery (https://github.com/amelielabs/monotone/commit/e361e5aaaf6df715f2fc5bde608edd5e5512dd4a)
* Improved crc32 calculation performance (https://github.com/amelielabs/monotone/commit/3613431cd3c1a05ab76455430bc9f93bc8332038)

## 1.1.0 (2025-02-13)

Changing the project license to MIT

## 1.0.0 (2024-04-16)

**The first public Monotone GA release.**

Embeddable Cloud-Native Storage for events and time-series data.

This release includes core functionality listed in the official
documentation at the moment of release.

https://monotone.studio

**Features**

- Automatic Range Partitioning
- Transparent Compression
- Transparent Encryption
- Multiple Storages
- Data Tiering
- Bottomless Storage
