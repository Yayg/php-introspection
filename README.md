# php-introspection

## How to compile

    $ phpize
    $ ./configure --enable-sqreen
    $ make
    $ sudo make install

Then edit your php.ini with the following line:

    extension=sqreen.so
