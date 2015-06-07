libtags
=======

A cross-platform library for reading tags, designed for highly constrained environments.

Comparison to id3lib:

```
╭────────────────┬─────────────────┬──────────────────┬──────────────────╮
│                │ libtags         │ id3lib           │ taglib           │
├────────────────┼─────────────────┼──────────────────┼──────────────────┤
│ ID3v2.4        │ yes             │ no               │ yes              │
│ Ogg/Vorbis     │ yes             │ no               │ yes              │
│ FLAC           │ yes             │ no               │ yes              │
│ replay gain    │ yes             │ no               │ ???              │
│ size           │ tiny            │ bloated          │ more bloated     │
│ license        │ MIT             │ LGPL             │ LGPL/MPL         │
│ written in     │ C               │ C++              │ C++              │
│ memory         │ no allocations  │ allocates memory │ allocates memory │
│ thread safe    │ yes             │ ???              │ ???              │
│ speed          │ ultra-fast      │ slow             │ fast             │
│ tag writing    │ no, not a goal  │ yes              │ yes              │
│ Plan 9 support │ yes, native     │ no               │ no               │
╰────────────────┴─────────────────┴──────────────────┴──────────────────╯
```
