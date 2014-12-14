# tmdbpp README

## INTRO

A C++ API wrapper for the TMDB (The Movie Database) Web API. See
https://www.themoviedb.org/documentation/api.

See COPYING for Copyright

## Current Status 

Supports 

* search().company("name",page);
* search().movie("title","language",page);
* search().collection("name",page);
* search().keyword("name",page)
* search().tv("name",page);
* search().tv().popular();

* get().movie(id)
* get().collection(id)
* get().company(id)
* get().keyword(id)
* get().tv(id)

CMakefile and other configs are under developed-
  
