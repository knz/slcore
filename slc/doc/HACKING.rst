====================
 SLC Hacking  guide
====================

:Abstract: This document presents tips & tricks for developers working
   on the SLC source code.

.. contents::

Getting started
===============

1. Obtain the top-level source tree (``sys``) from a public Git branch.

2. Read ``slc/doc/README.txt`` and ``slc/doc/HACKING.txt`` thoroughly.

3. Ensure you did #2, really. 

4. Check all the requirements are available, install/upgrade as
   needed. Use the installer from the separate ``deploy`` package.

5. Run::

     ./bootstrap
     mkdir build
     cd build
     ../configure 
     make
     make check

6. Enjoy!

Special requirements
--------------------

When working from the SCM repository, the following *additional*
requirements apply:

- Autoconf 2.68 or later

- Automake 1.11 or later

- Docutils 0.6 or later

- GNU Texinfo 4.13 or later


Working from the source directory
=================================

The ``slc`` and ``slr`` scripts are generated during
``make``. However, they contain absolute paths relative to their
installation directory. This means they cannot be used "as-is" before
``make install`` is run.

It would be a hassle during development to re-run ``make install``
after each change. Instead, it is possible to set environment
variables to indicate that the scripts should find the data in the
source directory. Use ``make show-vars`` at the top-level source
directory to set environment variables::

   $ eval `make show-vars`

Note: this will update ``PATH`` as well. Once the variables are set,
the command-line utilities can be used directly without running ``make
install``.

Committing work
===============

When pushing your changes to a remote directory:

1. ensure that ``make distcheck`` succeeds.

2. ensure that the appropriate ``ChangeLog`` contains a detailed
   description of your changes for each commit step you are
   sending. [#]_

   .. [#] Emacs users: use ``C-x 4 a`` from an edited file to get the
      entry automatically generated. Vim users: see
      http://www.vim.org/scripts/script.php?script_id=1631

   .. note:: There may be several change logs, pick the one recursively
      nearest to each change to document them. Strip the beginning of
      the path to each modified file relative to the location of the
      ChangeLog. (see previous ChangeLog entries and follow the
      pattern).

3. save the output of ``git format-patch`` to a file.

4. commit your set(s) of changes, one per software package. The
   content of the commit message should be the latest ``ChangeLog``
   entry, *without* the time stamp, but *with* a prefix to the first
   line identifying which component is being modified
   (e.g. ``[sl-core]``, ``[sl-programs]``, etc).

5. send the contents of the file from step #3 to the mailing list,
   ensuring that the changes to the change log(s) appear first in the
   e-mail. Alternatively, if using GitHub, advertise a pull request.


Distributing
============

To package a copy of the entire SL toolchain for final users, use the
``dist`` packager included in the separate ``deploy`` repository.
This packager automates the following steps:

1. ensure that ``make distcheck`` succeeds.

2. run ``make dist``.

After step #2 has completed, ``make dist`` will have output two
archives (``.tar.gz`` and ``.tar.bz2``) suitable for distribution.

Goodies
=======

The following "features" of the source tree are there for the benefit
of the developer, not the end user of the SL toolchain:


- ``make check-slt`` at the top directory invokes ``slt`` on the
  console, and lets it use ASCII art for test results. 

  This is nicer to the eye than ``make check``, but ``make check -jN``
  can deliver test results much faster on many-core machines. In both
  cases, failed tests cause log files to stay around for further
  investigation. Fish for the log files in the test subdirectories
  (extension ``.log``), or look at what ``slt`` says about remaining
  files;

- the list of SL implementations that are tested by ``make check`` and
  ``make check-slt`` is normally derived from ``configure`` flags
  (e.g. ``--enable-check-utc``), but can be overriden with the
  environment variable ``SLT_IMPL_LIST``;

- the environment variable ``TRACE``, when set and not empty, cause
  the scripts ``slt``, ``slc``, ``slr`` and ``timeout`` to enable the
  shell tracing mode (``set -x``). 

  .. note:: This feature is redundant with the better-looking output
     of ``slc -v``, ``slr -t`` and ``slt`` 's detailed logs. Check the
     documentation of these utilities for details. However, ``TRACE``
     was introduced when debugging process management and signal
     handling in ``slt`` and may still be useful to debug the script
     internals.

.. Local Variables:
.. mode: rst
.. End:
