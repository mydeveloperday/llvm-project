# Experimental LLVM Compiler Infrastructure

[![AppVeyor Status](https://ci.appveyor.com/api/projects/status/github/mydeveloperday/llvm-project?branch=master&svg=true)](https://ci.appveyor.com/project/mydeveloperday/llvm-project)

This fork of the LLVM project is to maintain experimental changes for clang-format (maybe and other tools in the future)

As there is often delay to land some revisions into clang-format upstream, this fork aims to promote those revisions by the production of a clang-format-experimental.exe binary.

This will allow the effectiveness of unlanded revisions to be evaluated without users having to work out how to merge a non committed change and go through the process of building clang-format.

This repo will contain current tip of master + revisions which appear to be wanted/desired by the wider community (revisions that have a large number of subscribers, Phabricator like tokens or are blocked from acceptance by lack of review or lack of consensous).

It may be revisions are blocked for legitimate reasons, But they may seem blocked for no real technical reason, except the view that either clang-format is "done" or that clang-format should not support too many options, or it that makes it hard for people to discover which options to use. But a search of github shows a worrying amount of usage of clang-format off/on in code which shows that using clang-format can be cumbersome if it doesn't quite match your style.  As a community I believe developers are often more than happy to search for and tweak a numerous set of options to get their code looking just right.

Code complexity aside, If all new options come with unit test  and those unit tests don't break or change existing tests then it 
should be safe to land such changes safely.

The purpose here is to give those unlanded revisions some velocity, pulling  those uncommitted revisions into a repo and using 
CI system (appveyor) to build a clang format binary for the  wider community to use, This may aid to get some capability landed upstream.

Inclusion into this repository, requires the same revisions submitted to phabricator and
those revisions have NOT yet been upstreamed. (we will get upstream revisions for free, as we sync with upstream master)

As the production of a release binary is based on committing to this repo, this may also help to reduce the time between new binaries being built on (https://llvm.org/builds/)

Unlanded Revisions will me merged by pulling those revision down, and building a commit in this fork using the following.

arc export --revision D12345 --git > revD12345.patch
patch -p1 < revD12345.patch

Unlanded revisions need to remain rebased to the HEAD of master to avoid excessive merge conflicts, and updates should go via the original revision (until landed)

I would like to follow the LLVM process of code review over in http://reviews.llvm.org.

If you see a revision over in reviews.llvm.org that you think should be included because its useful then raise an 
issue here citing the revision number D1234

# Revisions Included

The following is a list of revisions included in clang-format-experimental, for documentation you currently need to visit the revision in Phabricator

* [D28462](https://reviews.llvm.org/D28462) - clang-format: Add new style option AlignConsecutiveMacros
