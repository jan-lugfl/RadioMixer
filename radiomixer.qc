<qconf>
 <name>radiomixer</name>
 <profile>radiomixer.pro</profile>
 <moddir>qcm</moddir>
 <dep type='alsa' />
 <dep type='jackd' />
 <dep type='mad' />
 <dep type='samplerate'>
	<required />
 </dep>
 <dep type='vorbis' />
 <arg name='enable-songdb'>Enable the OpenRadio Song databse features. (optional)</arg>
 <arg name='enable-hwmixer'>Enable support for the OpenRadio hardware mixing device. (optional)</arg>
</qconf>
