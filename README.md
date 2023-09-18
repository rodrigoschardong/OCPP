# OCPP
Exploring and validating OCPP protocol

.. image:: https://github.com/mobilityhouse/ocpp/actions/workflows/pull-request.yml/badge.svg?style=svg
   :target: https://github.com/mobilityhouse/ocpp/actions/workflows/pull-request.yml

.. image:: https://img.shields.io/pypi/pyversions/ocpp.svg
   :target: https://pypi.org/project/ocpp/

.. image:: https://img.shields.io/readthedocs/ocpp.svg
   :target: https://ocpp.readthedocs.io/en/latest/

OCPP
----

Python package implementing the JSON version of the Open Charge Point Protocol
(OCPP). Currently OCPP 1.6 (errata v4), OCPP 2.0 and OCPP 2.0.1 (Final Version)
are supported.

You can find the documentation on `rtd`_.

Installation
------------

You can either the project install from Pypi:

.. code-block:: bash

   $ pip install ocpp

Or clone the project and install it manually using:

.. code-block:: bash

   $ pip install .

Quick start
-----------

Below you can find examples on how to create a simple OCPP 2.0 central system as
well as an OCPP 2.0 charge point.

.. note::

   To run these examples the dependency websockets_ is required! Install it by running:

   .. code-block:: bash

      $ pip install websockets
