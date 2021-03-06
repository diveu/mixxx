#ifndef KEYUTILS_H
#define KEYUTILS_H

#include <QString>
#include <QList>
#include <cmath>

#include "proto/keys.pb.h"
#include "track/keys.h"

class KeyUtils {
  public:
    enum KeyNotation {
        // The default notation (set with setNotation).
        DEFAULT = 0,
        OPEN_KEY = 1,
        LANCELOT = 2,
        TRADITIONAL = 3,
    };

    static QString keyDebugName(mixxx::track::io::key::ChromaticKey key);

    static inline bool keyIsMajor(mixxx::track::io::key::ChromaticKey key) {
        return key > mixxx::track::io::key::INVALID &&
                key < mixxx::track::io::key::C_MINOR;
    }

    // Returns the tonic, 0-indexed.
    static inline int keyToTonic(mixxx::track::io::key::ChromaticKey key) {
        if (key == mixxx::track::io::key::INVALID) {
            return mixxx::track::io::key::INVALID;
        }
        return static_cast<int>(key) - (keyIsMajor(key) ? 1 : 13);
    }

    // Takes a 0-indexed tonic and whether it is major/minor and produces a key.
    static inline mixxx::track::io::key::ChromaticKey tonicToKey(int tonic, bool major) {
        return static_cast<mixxx::track::io::key::ChromaticKey>(
            tonic + (major ? 1 : 13));
    }

    static QString keyToString(mixxx::track::io::key::ChromaticKey key,
                               KeyNotation notation=DEFAULT);

    static mixxx::track::io::key::ChromaticKey keyFromNumericValue(double value);

    static double keyToNumericValue(mixxx::track::io::key::ChromaticKey key);

    static QPair<mixxx::track::io::key::ChromaticKey, double> scaleKeyOctaves(
        mixxx::track::io::key::ChromaticKey key, double scale);

    static mixxx::track::io::key::ChromaticKey scaleKeySteps(
        mixxx::track::io::key::ChromaticKey key, int steps);

    static inline double stepsToOctaveChange(int steps) {
        return static_cast<double>(steps) / 12.0;
    }

    static int shortestStepsToKey(mixxx::track::io::key::ChromaticKey key,
                                  mixxx::track::io::key::ChromaticKey target_key);

    static int shortestStepsToCompatibleKey(mixxx::track::io::key::ChromaticKey key,
                                            mixxx::track::io::key::ChromaticKey target_key);

    // Returns a list of keys that are harmonically compatible with key using
    // the Circle of Fifths (including the key itself).
    static QList<mixxx::track::io::key::ChromaticKey> getCompatibleKeys(
        mixxx::track::io::key::ChromaticKey key);

    static mixxx::track::io::key::ChromaticKey guessKeyFromText(const QString& text);

    static mixxx::track::io::key::ChromaticKey calculateGlobalKey(
        const KeyChangeList& key_changes, int iTotalSamples);

    static void setNotation(
        const QMap<mixxx::track::io::key::ChromaticKey, QString>& notation);

    // Returns pow(2, octaveChange)
    static inline double octaveChangeToPowerOf2(const double& octaveChange) {
        // Some libraries (e.g. SoundTouch) calculate pow(2, octaveChange)
        // using this identity:
        // x^y = e^(y*ln(x))
        // Presumably because exp() is faster than pow() on some machines.
        // TODO(rryan) benchmark on Intel / AMD / ARM processors and pick accordingly.
        //static const lg2 = log(2); // 0.69314718056f
        //return exp(lg2 * octaveChange);
        return pow(2, octaveChange);
    }

    static inline double powerOf2ToOctaveChange(const double& power_of_2) {
        // log2 is in the C99 standard, MSVC only supports C90.
#ifdef _MSC_VER
        static const double lg2 = log(2.0);
        return log(power_of_2) / lg2;
#else
        return log2(power_of_2);
#endif
    }

    static mixxx::track::io::key::ChromaticKey openKeyNumberToKey(int openKeyNumber, bool major);

    static inline int keyToOpenKeyNumber(mixxx::track::io::key::ChromaticKey key) {
        switch (key) {
            case mixxx::track::io::key::C_MAJOR:
            case mixxx::track::io::key::A_MINOR:
                return 1;
            case mixxx::track::io::key::G_MAJOR:
            case mixxx::track::io::key::E_MINOR:
                return 2;
            case mixxx::track::io::key::D_MAJOR:
            case mixxx::track::io::key::B_MINOR:
                return 3;
            case mixxx::track::io::key::A_MAJOR:
            case mixxx::track::io::key::F_SHARP_MINOR:
                return 4;
            case mixxx::track::io::key::E_MAJOR:
            case mixxx::track::io::key::C_SHARP_MINOR:
                return 5;
            case mixxx::track::io::key::B_MAJOR:
            case mixxx::track::io::key::G_SHARP_MINOR:
                return 6;
            case mixxx::track::io::key::F_SHARP_MAJOR:
            case mixxx::track::io::key::E_FLAT_MINOR:
                return 7;
            case mixxx::track::io::key::D_FLAT_MAJOR:
            case mixxx::track::io::key::B_FLAT_MINOR:
                return 8;
            case mixxx::track::io::key::A_FLAT_MAJOR:
            case mixxx::track::io::key::F_MINOR:
                return 9;
            case mixxx::track::io::key::E_FLAT_MAJOR:
            case mixxx::track::io::key::C_MINOR:
                return 10;
            case mixxx::track::io::key::B_FLAT_MAJOR:
            case mixxx::track::io::key::G_MINOR:
                return 11;
            case mixxx::track::io::key::F_MAJOR:
            case mixxx::track::io::key::D_MINOR:
                return 12;
            default:
                return 0;
        }
    }

  private:
    static QMutex s_notationMutex;
    static QMap<mixxx::track::io::key::ChromaticKey, QString> s_notation;
    static QMap<QString, mixxx::track::io::key::ChromaticKey> s_reverseNotation;
};

#endif /* KEYUTILS_H */
