#ifndef FRAME_H
#define FRAME_H

#include "model/activmodel.h"

class Frame
{
private:
    // Speicherung der einzelnen Objekte im Farme
    std::vector<ActivModel> mObjects;
    // die Frame-Nummer
    size_t mFrameNr;
public:
    Frame();
    Frame(size_t frameNr);

    void clear();

    size_t getFrameNr();
    size_t getObjectSize();
    // gibt das ActivModel an Position objectID zurück
    ActivModel getObject(size_t ob_pos);
    // gibt die Vectorposition von zwei Objekten zurück die die selbe ObjectID haben (zweimal selbes Objekt in Frame)
    std::vector<size_t> SeveralTimesObjects();
    // git die Vectorposition von Objekten zurück, die in etwa die selbe Position wie gegeben haben
    std::vector<size_t> getObjectOnPosition(int x, int y, int w, int h, int acc);

    void addObject(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man);
    void addObject(ActivModel model);
    int getObjectID(size_t pos);
    void setObjectID(int lastO_id, int newO_id);
    bool existObject(int O_id);
    void deleteObject(int id);

    void deleteActionEvent(size_t pos);

    int getEventID(int O_id);
    void setEventID(int O_id, int E_id);
    bool existEvent(int E_id);
    void deleteEvent(int id);


    void setLandmarks(int O_id, double marks[5][2]);
    void setOrientation(int O_id, double orien[3]);
    void setPosition(int O_id, double pos[3]);
    void setProjection(int O_id, double proj[4]);
    void setRect(int O_id, int x, int y, int w, int h);

    QString printAll(size_t pos);
};

#endif // FRAME_H
