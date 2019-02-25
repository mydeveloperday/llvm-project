# Experimental LLVM Compiler Infrastructure

![AppVeyor Status](https://ci.appveyor.com/api/projects/status/github/mydeveloperday/llvm-project?branch=master&svg=true)

This fork of the LLVM project is to maintain experimental changes for clang-format and other tools.

There seems to be reluctance to accept some revisions into clang-format, citing that the clang-format 
shouldn't support every formatting option or that some revisions impact development costs.

However there are revisions which seem desired by the wider community (large number of subscribers, Phabricator like tokens), however those revisions seem blocked. 

It may be they are blocked for legitimate reasons, It seems however that there is often no real technical reason for blocking, except perhaps a view by some that clang-format is "done" or that we don't want to support too many options, because that makes it hard for people to discover which options to use.

However a search of github shows a worrying amount of usage of clang-format off/on in code which shows that using clang-format can be cumbersome if it doesn't quite match your style. 

As a community I believe developers are more than happy to search for a tweak a numerous set of options to get their code looking just right.

Code complexity aside, I see new options as giving power to people to choose. If all new options come with unit test 
and those unit tests don't break or change existing tests then it should be safe to land such changes safely.

My personal opinion is we should try and resolve those uses of clang-format on/off and where possible bring in 
other options to give users the flexibility to configure how they want to and not be restricted by the "code owners" view.

The purpose here is to give those unlanded revisions some velocity, pulling 
those uncommitted revisions into a repo and using CI system (appveyor) to build a clang format binary for the 
wider community that supports them.

My hope would be that, usage and external pressure would then apply pressure for the capability to be landed upstream.

Inclusion into this repository, requires revisions submitted to phabricator and
those revisions have not been upstreamed. (we will get those for free, as we sync with master)

I would like to follow the LLVM process of code review over in http://reviews.llvm.org. Ultimately I only want to land here what seems
reasonable but is not being accepted for what seems like non obvious reasons, hopefully this will give people a path to a prebuilt binary (from tip of master) that they can use.

If you see a revision over in reviews.llvm.org that you think should be included because its useful then raise an 
issue here citing the revision number D1234


# Revisions Included

The following is a list of revisions included in clang-format-experimental, for documentation you currently need to visit the revision in Phabricator

* [D28462](https://reviews.llvm.org/D28462) - clang-format: Add new style option AlignConsecutiveMacros
