/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005, 2006 by Jan Boysen                                *
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
#include "playerchannelfile.h"

playerChannelFile::playerChannelFile(QObject *parent, const char *name)
 : playerChannelStd(parent,name)
{
	bufferThread = new channelBufMngr(this);
	fileOpen = FALSE;
	loopMode = FALSE;
	decoder = NULL;
}

playerChannelFile::~playerChannelFile()
{
	if(!isStopped())
		stop();
	bufferThread->terminate();
}

void playerChannelFile::open( playListItem* track )
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
				state = 3;

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

void playerChannelFile::decode( )
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

void playerChannelFile::checkBuffer( )
{
	if( soundBuffers[0].canWrite( 1024 ) && isPlaying() )
	{
		decode();
	}
}

void playerChannelFile::stop( )
{
	if( isStopped() )
		return;
	playerChannelStd::stop();
	if( fileOpen )
		dynamic_cast<playListItem*>(meta)->stopped();
	close();
	emit( stopped() );
}

void playerChannelFile::close( )
{
	if(fileOpen)
	{
		delete decoder;
		decoder = NULL;
		fileOpen = FALSE;
	}
}

const bool playerChannelFile::isFileOpen( )
{
	return fileOpen;
}

const int playerChannelFile::getRTime( )
{
	return decoder->getRTime();
}

const int playerChannelFile::getTime( )
{
	return decoder->getTime();
}

void playerChannelFile::setName( QString newName )
{
	playerChannelStd::setName( newName );
	emit( nameChanged( newName ) );
	emit( refreshed() );
}

void playerChannelFile::play( )
{
	if( !fileOpen || isPlaying() )
		return;
	playerChannelStd::play();
	if( fileOpen )
		dynamic_cast<playListItem*>(meta)->startPlaying();
	emit( newMeta( *meta ) );
	emit( playing() );
}

const float playerChannelFile::getPosition_Samples( )
{
	return decoder->getPosition_Samples();
}

const float playerChannelFile::getTotal_Samples( )
{
	return decoder->getTotal_Samples();
}

const float playerChannelFile::getTotalFrames( )
{
	return decoder->getTotalFrames();
}

const float playerChannelFile::getPlayedFrames( )
{
	return decoder->getPlayedFrames();
}

const float playerChannelFile::getRemainFrames( )
{
	return decoder->getRemainFrames();
}

void playerChannelFile::pause( )
{
	// pausing if we are not in Play mode makes no sence............
	if( !isPlaying() )
		return;
	playerChannelStd::pause();
	emit( paused() );
}

const bool playerChannelFile::isLooping( )
{
	return loopMode;
}

void playerChannelFile::toggleLoop( )
{
	if( loopMode )
		loopMode = FALSE;
	else
		loopMode = TRUE;
}

const float playerChannelFile::getPrerollFrames( )
{
	return meta->getPreLength().second()*25;
}
