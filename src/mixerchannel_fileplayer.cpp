/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2009 by Jan Boysen                                *
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
#include "mixerchannel_fileplayer.h"

mixerChannel_filePlayer::mixerChannel_filePlayer( const char *name )
 : mixerChannel(name)
{
	bufferThread = new channelBufMngr(this);
	fileOpen = FALSE;
	loopMode = FALSE;
	decoder = NULL;
}

mixerChannel_filePlayer::~mixerChannel_filePlayer()
{
	if(!isStopped())
		stop();
	bufferThread->terminate();
}

void mixerChannel_filePlayer::open( playListItem* track )
{
	meta = track;

	soundBuffers[0].flush();
	soundBuffers[1].flush();

	if(fileOpen)
		close();

	fHandle = fopen( track->getFile(), "r");
	if( !fHandle )
	{
		qWarning(tr("file open error.. proberly not found..")+track->getFile());
		return;
	}
	fileName = track->getFile();

	try
	{
		QRegExp rx( "^(.*)\\.(\\w+)$" );
		if ( rx.search( track->getFilename() ) != -1 ) {
#ifdef HAVE_VORBIS
			if( rx.cap(2).lower() == "ogg" )
			{
				qWarning( tr("OGG/Vorbis File detected") );
				oggDecoder::readMetaFromFile( track );
				decoder = new oggDecoder( fHandle, this, "OGGDecoder" );
			}
#endif
#ifdef HAVE_MAD
			if( rx.cap(2).lower() == "mp3" )
			{
				qWarning( tr("MPEG-1 Layer III File detected") );
				mpgDecoder::readMetaFromFile( track );
				decoder = new mpgDecoder( fHandle, this, "MPGDecoder" );
			}
#endif
			if( decoder == NULL )
			{
				QMessageBox::warning( NULL, tr("RadioMixer - Playerchannel File"), tr("unknown Filetype: ")+rx.cap(2).lower(), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
				fileOpen = FALSE;
			}else{
//				decoder->setMetaInfos( &meta );
				smplRate = track->getSamplerate();
				channels = track->getChannels();
				fileOpen = TRUE;
				if( meta )
					dynamic_cast<playListItem*>(meta)->cueing();
				emit( cued( *meta ) );
                                setState( 3 );

				if(!bufferThread->running())
					bufferThread->start();
				emit( refreshed() );
			}
		}
	}
	catch( decoderException* ex )
	{
		qWarning( ex->msg );
		fileOpen = FALSE;
	}
}

void mixerChannel_filePlayer::decode( )
{
	bool endOfTrack = 0;
	float** decBuff = new float*[2];		//Buffer for Sound Decoder
	int dataFetched;
	unsigned int toFetch = soundBuffers[0].getFree();

	dataFetched = decoder->decode( &decBuff, toFetch );
	if(dataFetched == 0)
	{
		if( loopMode )
		{
			decoder->reset();
			dataFetched = decoder->decode( &decBuff, toFetch );
			if(dataFetched == 0)
				stop();
		}
		else
			stop();
		endOfTrack = 1;
	}else
	{
		// process Audio....
		//mix Monot to Stereo
		if( channels == 1 )
		{
			decBuff[1] = new float[dataFetched];
			for(int i=0; i<(dataFetched); i++)
			{
				decBuff[1][i] = decBuff[0][i];
			}
		}
		//mix volumes
		for(int i=0; i<(dataFetched); i++)
		{
			decBuff[0][i] *= volume * volume_left;
			decBuff[1][i] *= volume * volume_right;
		}
		//Write Data to ringbuffer
		soundBuffers[0].write( decBuff[0], dataFetched );
		soundBuffers[1].write( decBuff[1], dataFetched );
	
		//deallocate Memory if we are mixing mono to strereo
		if( channels == 1 )
			delete decBuff[1];
	}
	if( endOfTrack )
		emit( trackEnded() );
}

void mixerChannel_filePlayer::checkBuffer( )
{
	if( soundBuffers[0].canWrite( 1024 ) && isPlaying() )
	{
		decode();
	}
}

void mixerChannel_filePlayer::stop( )
{
	if( isStopped() )
		return;
        mixerChannel::stop();
	if( fileOpen )
		dynamic_cast<playListItem*>(meta)->stopped();
	close();
	emit( stopped() );
}

void mixerChannel_filePlayer::close( )
{
	if(fileOpen)
	{
		delete decoder;
		decoder = NULL;
		fileOpen = FALSE;
	}
}

void mixerChannel_filePlayer::cue()
{
    //TODO: implement me !!! ( move cueNewTrack function here )
}

const bool mixerChannel_filePlayer::isFileOpen( )
{
	return fileOpen;
}

const int mixerChannel_filePlayer::getRTime( )
{
	return decoder->getRTime();
}

const int mixerChannel_filePlayer::getTime( )
{
	return decoder->getTime();
}

void mixerChannel_filePlayer::setName( QString newName )
{
        mixerChannel::setName( newName );
	emit( nameChanged( newName ) );
	emit( refreshed() );
}

void mixerChannel_filePlayer::play( )
{
	if( !fileOpen || isPlaying() )
		return;
        mixerChannel::play();
	if( fileOpen )
		dynamic_cast<playListItem*>(meta)->startPlaying();
	emit( newMeta( *meta ) );
	emit( playing() );
}

const float mixerChannel_filePlayer::getPosition_Samples( )
{
	return decoder->getPosition_Samples();
}

const float mixerChannel_filePlayer::getTotal_Samples( )
{
	return decoder->getTotal_Samples();
}

const float mixerChannel_filePlayer::getTotalFrames( )
{
	return decoder->getTotalFrames();
}

const float mixerChannel_filePlayer::getPlayedFrames( )
{
	return decoder->getPlayedFrames();
}

const float mixerChannel_filePlayer::getRemainFrames( )
{
	return decoder->getRemainFrames();
}

void mixerChannel_filePlayer::pause( )
{
	// pausing if we are not in Play mode makes no sence............
	if( !isPlaying() )
		return;
        mixerChannel::pause();
	emit( paused() );
}

const bool mixerChannel_filePlayer::isLooping( )
{
	return loopMode;
}

void mixerChannel_filePlayer::toggleLoop( )
{
	if( loopMode )
		loopMode = FALSE;
	else
		loopMode = TRUE;
}

const float mixerChannel_filePlayer::getPrerollFrames( )
{
	return meta->getPreLength().second()*25;
}
