/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2007 by Jan Boysen                                *
 *   trekkie@media-mission.de                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "soundplayeralsa.h"

soundPlayerALSA::soundPlayerALSA( )
 : soundPlayer()
{
	alsaPlayer = new alsaPlayerThread( this );
}

soundPlayerALSA::~ soundPlayerALSA( )
{
}

void soundPlayerALSA::open( QString device )
{
	alsaPlayer->open( device );
	alsaPlayer->start();
}

void soundPlayerALSA::close( )
{
}

void soundPlayerALSA::interleave( )
{
	float Left, Right;
	for(unsigned int i=0;i < interMixSamples;i++)
	{
		outputBuffers[0].read( &Left, 1 );
		outputBuffers[1].read( &Right, 1);
		outputBuffer[i*2] = int(Left*32000);
		outputBuffer[(i*2)+1] = int(Right*32000);
	}
}

alsaPlayerThread::alsaPlayerThread( soundPlayerALSA* alsaPlayer )
{
	this->alsaPlayer = alsaPlayer;
	alsaPlayer->outRate = 48000;
	alsaPlayer->outputBuffer=new short[2048];
}

alsaPlayerThread::~alsaPlayerThread()
{
}

void alsaPlayerThread::open( QString device )
{
	if(alsaPlayer->devOpened)
	{
		QMessageBox::warning( NULL, QObject::tr("RadioMixer - ALSA"), QObject::tr("ALSA Device \"%1\" allready opened..").arg(device), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}else
	{
#ifdef HAVE_ALSA
 		snd_device = device;
		/* Allocate the snd_pcm_hw_params_t structure on the stack. */
		snd_pcm_hw_params_alloca(&hwparams);

		// open the device
		if (snd_pcm_open(&alsa_handle, snd_device.ascii() , stream, 0) < 0)
		{
			QMessageBox::warning( NULL, QObject::tr("RadioMixer - ALSA"), QObject::tr("Error opening PCM device %1").arg(device), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}

		/* Init hwparams with full configuration space */
		if (snd_pcm_hw_params_any(alsa_handle, hwparams) < 0)
		{
			QMessageBox::warning( NULL, QObject::tr("RadioMixer - ALSA"), QObject::tr("Can not configure this PCM device."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}

		if (snd_pcm_hw_params_set_access(alsa_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
		{
			QMessageBox::warning( NULL, QObject::tr("RadioMixer - ALSA"), QObject::tr("Error setting access."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}

		/* Set sample format */
		if (snd_pcm_hw_params_set_format(alsa_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0)
		{
			QMessageBox::warning( NULL, QObject::tr("RadioMixer - ALSA"), QObject::tr("Error setting format."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}

		/* Set sample rate. If the exact rate is not supported */
		/* by the hardware, use nearest possible rate.         */ 
		unsigned int origRate = alsaPlayer->outRate;
		if (snd_pcm_hw_params_set_rate_near(alsa_handle, hwparams, &origRate, 0) < 0)
			qWarning( QObject::tr("Error setting rate.") );

		if (origRate != alsaPlayer->outRate)
			qWarning( QObject::tr("The rate %1Hz is not supported by your hardware.\nUsing %2Hz instead.").arg(origRate).arg(alsaPlayer->outRate) );

		/* Set number of channels */
		if (snd_pcm_hw_params_set_channels(alsa_handle, hwparams, 2) < 0)
		{
			QMessageBox::warning( NULL, QObject::tr("RadioMixer - ALSA"), QObject::tr("Error setting channels."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}

		/* Set number of periods. Periods used to be called fragments. */ 
		if (snd_pcm_hw_params_set_periods(alsa_handle, hwparams, periods, 0) < 0)
			qWarning( QObject::tr("Error setting periods.") );

		/* Set buffer size (in frames). The resulting latency is given by */
		/* latency = periodsize * periods / (rate * bytes_per_frame)     */
		if (snd_pcm_hw_params_set_buffer_size(alsa_handle, hwparams, (periodsize * periods)>>2) < 0)
			qWarning( QObject::tr("Error setting buffersize.") );

		/* Apply HW parameter settings to */
		/* PCM device and prepare device  */
		if (snd_pcm_hw_params(alsa_handle, hwparams) < 0)
			qWarning( QObject::tr("Error setting HW parms.") );
		alsaPlayer->devOpened = TRUE;
		qDebug( QObject::tr("ALSA Device \"%1\"opened: ").arg(device));
#endif
	}
}

void alsaPlayerThread::run()
{
	while(1)
	{
		bool playing = FALSE;
                mixerChannelManager::storageType::iterator it;
                for( it = mixerChannelManager::inChannels.begin(); it != mixerChannelManager::inChannels.end(); it++ )
		{
//			if( (*it)->isPlaying() )
//				playing = TRUE;
		}
		if( playing )
		{
#ifdef HAVE_ALSA
			//reset Alsa RingBuffer if ALSA has detected an Bufferunrerun.......
			if( snd_pcm_state(alsa_handle) == SND_PCM_STATE_XRUN )
				snd_pcm_prepare(alsa_handle);
			alsaPlayer->mixChannels();
			alsaPlayer->interleave();
			snd_pcm_writei(alsa_handle, alsaPlayer->outputBuffer, 1024);
			msleep(5);
#endif
		}else
		{
			msleep(100);
		}
	}
	qDebug( QObject::tr("ALSA Thread is exiting....."));
}


/**
 * This static functions return a Stringliste of all ALSA Soundcard devices in the system
 * @return a QStringlist with all found ALSA Sounddevices
 */
QStringList soundPlayerALSA::getDevices( )
{
#ifdef HAVE_ALSA
	// This Code has been inspired by the ALSA Outplugplugin of XMMS.....
	int err=0;
	int card=-1;
	QStringList ret;
	ret.append("default (default ALSA device)");

	if ((err = snd_card_next(&card)) != 0)
	{
		qWarning(" soundPlayerALSA::getDevices( ): snd_next_card() failed: "+QString(snd_strerror(-err)));
	}
	while( card > -1)
	{
		int pcm_device = -1;
		snd_ctl_t* ctl;
		char* card_name;

		if ((err = snd_card_get_name(card, &card_name)) != 0)
		{
			qWarning(" soundPlayerALSA::getDevices( ): snd_card_get_name() failed: "+QString( snd_strerror(-err)));
			card_name = "Unknown soundcard";
		}

		if ((err = snd_ctl_open(&ctl, QString("hw:"+QString::number(card)).ascii(), 0)) < 0)
		{
			qWarning(" soundPlayerALSA::getDevices( ): snd_ctl_open() failed: "+QString( snd_strerror(-err)));
			return ret;
		}

		for(;;)
		{
			snd_pcm_info_t* pcm_info;
			snd_pcm_info_alloca(&pcm_info);
			if ((err = snd_ctl_pcm_next_device(ctl, &pcm_device)) < 0)
			{
				qWarning(" soundPlayerALSA::getDevices( ): snd_ctl_pcm_next_device() failed: "+QString(snd_strerror(-err)));
				pcm_device = -1;
			}
			if (pcm_device < 0)
				break;

			snd_pcm_info_set_device(pcm_info, pcm_device);

			if( (err = snd_ctl_pcm_info(ctl, pcm_info)) < 0)
			{
				qWarning(" soundPlayerALSA::getDevices( ): snd_ctl_pcm_info() failed: "+QString(snd_strerror(-err))+"   at hw:"+QString::number(card)+","+QString::number(pcm_device));
				continue;
			}
			ret.append("hw:"+QString::number(card)+","+QString::number(pcm_device)+" ("+QString(card_name)+" -> "+QString(snd_pcm_info_get_name(pcm_info))+")");
		}
		snd_card_next(&card);
	}
	return ret;
#endif
}

