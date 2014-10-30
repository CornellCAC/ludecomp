import sys
import urllib2

if __name__ == "__main__":

    (library, compiler, runtime, jobid, user) = sys.argv[1:]
    (user,jobid,runtime,compiler,library) = \
             map(urllib2.quote,(user,jobid,runtime,compiler,library))
    base_url="http://consultrh5.cac.cornell.edu/emc256/intro_to_ranger/?"
    args="user="+user+"&jobid="+jobid+"&run_time="+runtime+\
          "&compiler="+compiler+"&library="+library+"&arguments=2000"
    print args

    print urllib2.urlopen(base_url+args).read()
