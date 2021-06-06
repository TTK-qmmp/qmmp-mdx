#include "mdxhelper.h"
#include "decoder_mdx.h"
#include "decodermdxfactory.h"

#include <QMessageBox>

bool DecoderMdxFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderMdxFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "MDX Plugin";
    properties.shortName = "mdx";
    properties.filters << "*.mdx" << "*.m";
    properties.description = "MDX Chiptunes Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderMdxFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderMdx(path);
}

QList<TrackInfo*> DecoderMdxFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    MdxHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        const QMap<Qmmp::MetaData, QString> metaData(helper.readMetaData());
        for(auto itr = metaData.begin(); itr != metaData.end(); ++itr)
        {
            info->setValue(itr.key(), itr.value());
        }
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "MDX");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderMdxFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderMdxFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

void DecoderMdxFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About MDX Reader Plugin"),
                        tr("Qmmp MDX Reader Plugin")+"\n"+
                        tr("Written by: Greedysky <greedysky@163.com>"));
}

QString DecoderMdxFactory::translation() const
{
    return QString();
}
