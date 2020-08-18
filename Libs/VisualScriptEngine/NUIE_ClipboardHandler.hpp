#ifndef NUIE_CLIPBOARDANDLER_HPP
#define NUIE_CLIPBOARDANDLER_HPP

#include "NUIE_Version.hpp"

#include <vector>
#include <memory>

namespace NUIE
{

class ClipboardHandler
{
public:
	ClipboardHandler ();
	virtual ~ClipboardHandler ();

	virtual NUIE::Version	GetCurrentVersion () const = 0;
	virtual bool			IsCompatibleVersion (const NUIE::Version& version) const = 0;

	virtual bool			HasClipboardContent () const = 0;
	virtual bool			GetClipboardContent (std::vector<char>& content) const = 0;
	virtual bool			SetClipboardContent (const std::vector<char>& content) = 0;
};

using ClipboardHandlerPtr = std::shared_ptr<ClipboardHandler>;
using ClipboardHandlerConstPtr = std::shared_ptr<const ClipboardHandler>;

class NullClipboardHandler : public ClipboardHandler
{
public:
	NullClipboardHandler ();
	virtual ~NullClipboardHandler ();

	virtual NUIE::Version	GetCurrentVersion () const override;
	virtual bool			IsCompatibleVersion (const NUIE::Version& version) const override;

	virtual bool			HasClipboardContent () const override;
	virtual bool			GetClipboardContent (std::vector<char>& content) const override;
	virtual bool			SetClipboardContent (const std::vector<char>& content) override;
};

class MemoryClipboardHandler : public ClipboardHandler
{
public:
	MemoryClipboardHandler ();
	virtual ~MemoryClipboardHandler ();

	virtual NUIE::Version	GetCurrentVersion () const override;
	virtual bool			IsCompatibleVersion (const NUIE::Version& version) const override;

	virtual bool			HasClipboardContent () const override;
	virtual bool			GetClipboardContent (std::vector<char>& content) const override;
	virtual bool			SetClipboardContent (const std::vector<char>& content) override;

private:
	std::vector<char>		clipboard;
};

}

#endif
