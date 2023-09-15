
RUN apt-get install -y \
    gdb \
    curl

# Add compiled bins and some data into the container
ADD bin /usr/local/bin/
ADD etc /usr/local/etc/
ADD sql /usr/local/share/artifact/sql/

# Declare volumes to share files between host and container (configuration file, logs, dbc, maps ....)
VOLUME /usr/local/etc/artifact /usr/local/opt/artifact /usr/local/var/log/artifact

ENTRYPOINT ["run-worldserver"]
